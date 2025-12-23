// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

struct Library         *CNetCBase    = NULL; // pointer to cnet.library lib base
struct Library         *CNetBase     = NULL; // pointer to cnet.library lib base
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
   /* ------------- YOUR CLEANUP CODE STARTS HERE - IF ANY ----------------- */







   /* -------------- CNETC DOOR SHUTDOWN PROCEDURE BEGINS ------------------ */
   if(context)
      {
      CNC_ShutDown(NULL);
      CNC_DisposeContext();
      context=NULL;
      }
   if(CNetCBase)
      {
      CloseLibrary(CNetCBase);
      CNetCBase=NULL;
      }
   if(CNetBase)
      {
      CloseLibrary(CNetBase);
      CNetBase=NULL;
      }
}



void main(int argc, char **argv)
{
	short index;
	char handlestr[22];

   /* ------------------ CHECK IF RAN AS A CNETC DOOR ---------------------- */
   if(argc < 2)
      {
      Printf("This is a CNetC Program!\n");
      return;
      }
   /* ------------------ CNETC DOOR SETUP STARTS HERE ---------------------- */
   atexit(GetOut); // an exit trap so that GetOut() is invoked when exit() is called.
   if( !(CNetCBase = OpenLibrary("cnetc.library", 4)) )
      GetOut();
   if( !(CNetBase = OpenLibrary("cnet.library", 4)) )
      GetOut();
   if( !(context = CNC_InitContext(argv[1])) )
      GetOut();

   z		=	context->z;
   myp	=	context->myp;
   /* --------------------- YOUR CODE BEGINS HERE -------------------------- */


	CNC_EnterLine(21, 0, "Enter handle to find: ");
	strcpy(handlestr, z->InBuffer);
	CNC_EnterLine(4, 0, "Enter optional serial id: ");
	if(FindHandle(&index, handlestr, atoi(z->InBuffer)))
		{
		sprintf(z->ABuffer, "\nindex: %d\n", index);
		CNC_PutA();
		sprintf(z->ABuffer, "\nmyp->IName[%d]: %ld\n", index, myp->IName[index]);
		CNC_PutA();

		sprintf(z->ABuffer, "\nmyp->IName[%d]: %ld\n", index, myp->IName[index]);
		CNC_PutA();

		sprintf(z->ABuffer, "\nmyp->Key[ %d ].Handle: %s\n", myp->IName[index]-1, myp->Key[ myp->IName[index]-1 ].Handle);
		CNC_PutA();
		}
	else
		{
		CNC_PutText("\nNot found!\n");
		}






   /* --------------------- END OF YOUR MAIN CODE -------------------------- */
   exit(0); // exit back to CNet - exit trap automatically calls GetOut()
}

