// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

struct Library         *CNetCBase    = NULL; // pointer to cnet.library lib base
struct CNetCContext    *context      = NULL; // pointer to context data, containing
                                             // cnet mainport, current portdata, etc,.
                                             // see include/cnetc_library.h for
                                             // structure information
struct PortData        *z            = NULL; // pointer to be initialized later to
                                             // context->z
struct MainPort        *myp          = NULL; // pointer to be initialized later to
                                             // context->myp
short                  portnum       = -1;


void DoDOSTOD(int argc, char **argv);


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
}



void main(int argc, char **argv)
{
   char *s;

   if(argc < 2)
      DoDOSTOD(argc, argv);
   /* ------------------ CNETC DOOR SETUP STARTS HERE ---------------------- */
   atexit(GetOut); // an exit trap so that GetOut() is invoked when exit() is called.
   if(!(CNetCBase = OpenLibrary("cnetc.library", 4)) )
      GetOut();
   if(!(context = CNC_InitContext(argv[1])) )
      {
      // WAS NOT RAN AS A CNETC DOOR - Use AmigaDOS shell version of
      // code instead.
      DoDOSTOD(argc, argv);
      }

   z     =   context->z;
   myp   =   context->myp;
   /* --------------------- YOUR CODE BEGINS HERE -------------------------- */


   // Parse input whether door is "run" from main or executed from BBSMENU
   if(argc > 2)
      portnum=atoi(argv[2]);         // AmigaDOS command line input
                                     // argv[0]    = door path/name
                                     // argv[1]    = door msg port name
                                     // argv[2]    = user argument 1
                                     // argv[x]    = user argument x
   else if(z->npitems > 1)
      portnum = atoi(z->pitem[1]);   // CNet command line input
                                     // pitem[0] = CNet command name
                                     // pitem[1] = user argument 1
                                     // pitem[x] = user argument x

   else if(z->InBuffer)              // remains of command in z->InBuffer
      {
      // parse the second argument (the first is the door command name)
      // from z->InBuffer by looking for first space
      if(s=strchr(z->InBuffer, ' '))
         {
         s++;
         portnum = atoi(s);
         }
      }

   // initial parsing didn't reveal user command line input; use EnterLine()
   if(portnum == -1)
      {
      if(!CNC_EnterLine(45, ELINE_INPUTBOX, "Port Number: "))
         exit(0);
      portnum=atoi(z->InBuffer);
      }

   // is screen open already?
   if(myp->PortZ[portnum]->Window)
      {
      // yes, pop to front?
      if(CNC_PutQ("\nScreen is already open; pop to front? [Y/n] ?1"))
         ScreenToFront(myp->PortZ[portnum]->Window->WScreen);
      }
   else
      {
      // trigger screen to open
      CNC_OpenDisplay(portnum);
      sprintf(z->ABuffer, "Triggered port %d screen to open.\n", portnum);
      CNC_PutA();
      }



   /* --------------------- END OF YOUR MAIN CODE -------------------------- */
   exit(0); // exit back to CNet - exit trap automatically calls GetOut()
}


// AmigaDOS version of Port screen open
void DoDOSTOD(int argc, char **argv)
{
   if(myp=(struct MainPort *)FindPort(CNETPORT)) // test for mainport since
      {                                          // it is slightly required here. :)
      if(argc < 2)
         {
         char buffer[10]; // this should be increased to accept a longer value but
                          // we'll assume the user KNOWS CNet only accepts 0-99.
                          // In practice, one should use a routine that LIMITS the
                          // number of input characters to the length of the input
                          // buffer (minus 1)..  but that is beyond the scope of
                          // this example.

         printf("Port number: ");
         gets(buffer);
         portnum = atoi(buffer);
         }
      else
         portnum = atoi(argv[1]);

		// Is window already open?         
      if(myp->PortZ[portnum]->Window)
         {
         // yes, pop to front?
         printf("Screen is already open; pop to front? [y/n] ");
         if(tolower(getch()) == 'y')
            {
            printf("\n");
            ScreenToFront(myp->PortZ[portnum]->Window->WScreen);
            }
			else printf("\n");
         }
      else
         {
         // no, trigger it to open! It will open it on the next service
         // call - usually in 3 second intervals.
         myp->pc[ portnum ].open |= 1;
         myp->PortZ[portnum]->WantToOpen = TRUE;
   
         printf("Triggered port %d screen to open\n", portnum);
         }
      }
   exit(0);
}
