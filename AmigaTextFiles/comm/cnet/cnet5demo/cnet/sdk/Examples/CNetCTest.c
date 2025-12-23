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
		CloseLibrary(CNet4Base);

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	LONG zzz=0;
	char date[40];

	if(!(CNet4Base = OpenLibrary("cnet4.library", 4L)))
		exit(RETURN_FAIL);

	CNC_PutText("\n");
	sprintf(z->ABuffer, "   Your Handle: %s\n", z->user1.Handle);
	CNC_PutA();
	sprintf(z->ABuffer, "Your Real name: %s\n", z->user1.RealName);
	CNC_PutA();
	sprintf(z->ABuffer, "      Your age: %d\n\n", CalculateAge(&z->user1));
	CNC_PutA();


	// ------------- MAKE PRINTABLE DATE OUT OF CNET IsDate STRUCTURE. --------------

	// CNetPrintDate() is a cnet4.library function.
	//
	CNetPrintDate(&z->user1.LastCall, date, NULL, NULL);
	sprintf(z->ABuffer, "     Last Call: %s\n", date);
	CNC_PutA();
	CNetPrintDate(&z->user1.Birthdate, date, NULL, NULL);
	sprintf(z->ABuffer, "     Birthdate: %s\n\n", date);
	CNC_PutA();
		
	
	// --------------------------- SHOW LOADED PORTS. -----------------------------

	CNC_PutText("Loaded ports\n---------------------\n");
	for(zzz=0;zzz<100;zzz++)
		{
		if(IsPortLoaded(zzz))
			{
			sprintf(z->ABuffer, "%3d: %s\n", zzz, IsPortOccupied(zzz) ? myp->PortZ[zzz]->user1.Handle : "[IDLE]");
			CNC_PutA();
			}
		}


	//  ---- SIMPLY PRINT A NEWLINE - SAME AS CNC_PutText("\n") OR CNC_PutText("n1") ---

	CNC_DoReturn();


	// ---------------- ASK USER IF HE/SHE WANTS TO DO AN EDITOR TEST. -------------

	if(CNC_PutQ("Do editor test now? [Y/n] ?1"))
		{
		// YES!

			//  ------ sample editor usage - load systext:menu/main into editor first. ---
			{
			BPTR fh;
			char edfile[128];

			// open existing source file
			if(fh=Open("systext:menu/main", MODE_OLDFILE))
				{
				// fill editor with contents of file belonging to file handle "fh".

				// PrepEditor() is a cnet.lib function.
				PrepEditor(fh);

				// Allow user to edit editor file.
				// allow 255 lines and use existing _edbuffX file contents.
				CNC_CallEditor(255, TRUE);

				// create the CNet editor filename for the current port
				// MakeEd() is a cnet.lib function
				MakeEd(edfile);

				// read the editor file and print file not found if it does not exist.
				CNC_ReadFile(edfile, TRUE);

				// close the source file.
				Close(fh);
				}
			}
		
		}
	else
		{
		// NO!
		CNC_PutText("Editor test skipped\n");
		}


	// ------------------------------- ENTERLINE USAGE -----------------------
	//
	{
	BOOL done = FALSE;

	while(!done)
		{
		if(CNC_EnterLine(10, ELINE_INPUTBOX, "\nEnter 'YES' or 'NO': ")) // see io.h for enterline flags
			{
			if(!stricmp(z->InBuffer, "YES"))
				{
				CNC_PutText("Alright, you agree - so what?!\n");
				done = TRUE; // flag to quit loop
				}
			else if(!stricmp(z->InBuffer, "NO"))
				{
				CNC_PutText("Awwwwww, you suck!\n");
				done = TRUE; // flag to quit loop
				}
			else
				{
				CNC_PutText("Uhmmm..  You DO know english, correct?\n");
				}
			}
		else
			{
			CNC_PutText("I said 'YES' or 'NO'!!\n");
			}
		}
	}


	// ---------------------------- ONEKEY USAGE ---------------------------
	{
	char vowels[] = "aeiou\0"; // string of characters
	short zzz=0; // temp variable
	char key;	
	char choices[2] = { 0,0 };

	while(vowels[zzz]) // while a valid character is indexed
		{
		do
			{
			// prepare a string to be printed in z->ABuffer
			sprintf(z->ABuffer, "\nPress '%c': ", vowels[zzz]);

			// Print z->ABuffer
			CNC_PutA();

			// get a key from the user
			// note that by default, CNC_OneKey returns UPPERCASE character input.
         // to make it return lowercase input, set z->NoCaseConvert = TRUE
         // and then set it back after getting your key input.

			z->NoCaseConvert = TRUE;
			key=CNC_OneKey();
			z->NoCaseConvert = FALSE;

			} while(key != vowels[zzz]);
		zzz++;
		}
	CNC_PutText("\nGood, you got through all the vowels!\n");

	// a similar version of the above code, using CNC_PutQMult()

	zzz=0;
	while(vowels[zzz]) // while a valid character is indexed
		{
		// prepare a string to be printed in z->ABuffer
		sprintf(z->ABuffer, "\nPress '%c': ", vowels[zzz]);
		choices[0] = vowels[zzz];

		// Print z->ABuffer
		z->NoCaseConvert = TRUE;
		key=CNC_PutQMult(z->ABuffer, choices);

		sprintf(z->ABuffer, "User pressed '%c'", choices[key-1]);
		CNC_PutA();

		z->NoCaseConvert = FALSE;
		zzz++;
		}
	CNC_PutText("\nGood, you got through all the vowels!\n");
	}


	return;
}
