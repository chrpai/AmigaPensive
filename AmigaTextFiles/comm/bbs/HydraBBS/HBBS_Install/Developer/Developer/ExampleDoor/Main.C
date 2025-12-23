/* **** HBBS Example Door Code********************************************** */

/*

   Introduction
   ============

   This is an example C source code, that can be compiled with SAS/C
   Use this to build all your doors with.

   If you use this source then you *must* mention my name in the
   documentation for your door, you *must* also send me a copy of the
   program that you have written, for FREE.  Via e-mail or post.

   Check the docs for my e-mail address, or check the HydraBBS web site:
   http://come.to/HydraBBS

   If you start writing a door please send me an e-mail so I know you are
   working on it, then I can let other's know that you are writng this door
   or I can let you know if someone is working on the same thing (then you
   could team up with them or vice versa..)

   Also, if you want some ideas for doors to write that need writing then
   please a) check the web site for the current list of wanted doors and
   b) e-mail me to see what I need doing or have had ideas about.

   Thanks

   Dominic Clifton - Author of HydraBBS!

   Notes
   =====

   Replace all occurences of the string DoorName with the name of your door!
   DoorName should not be more than 15 chars if possible otherwise it won't
   fit in things like the WHO door's display...

   This code is heavily documented, so you should be able to figure out
   what's what.

*/

/*
  DoorName
  ========

    what the door does

  Version
  =======

    x.xx, release xx, dd/mm/yyyy

  Options
  =======

    N_ND->ActiveDoor->SystemOptions
    -------------------------------



    Command Line Arguments
    ----------------------

    2  <option>

    3  ...

    4  .

  ToDo
  ====

    what you still have to do



*/

/* **** Includes *********************************************************** */

#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <HBBS/ANSI_Codes.h>
#include <HBBS/Defines.h>
#include <HBBS/Access.h>
#include <HBBS/types.h>
#include <HBBS/structures.h>

#include <HBBS/Hbbscommon_protos.h>
#ifdef __SASC
#include <HBBS/hbbscommon_pragmas_sas.h>
#else
#include <HBBS/hbbscommon_pragmas_stc.h>
#endif

#include <HBBS/Hbbsnode_protos.h>
#ifdef __SASC
#include <HBBS/Hbbsnode_pragmas_sas.h>
#else
#include <HBBS/Hbbsnode_pragmas_stc.h>
#endif

#include <HBBS/release.h>
char *versionstr="$VER: DoorName "RELEASE_STR;

/* **** Variables ********************************************************** */


struct Library *HBBSCommonBase  = NULL;
struct Library *HBBSNodeBase    = NULL;

struct BBSGlobalData *BBSGlobal = NULL;
struct NodeData *N_ND           = NULL;
int    N_NodeNum                = -1;
char   outstr[1024];

long __stack=16*1024; // increse this in 4k incrments if you suffer from
                      // random/suprious crashings after or during the running
                      // of your door.

int    gargc;         // these are just copies of main()'s argc and argv..
char   **gargv;

/* **** Functions ********************************************************** */


#ifdef __SASC
int CXBRK(void) { return(0); }
int _CXBRK(void) { return(0); }
void chkabort(void) {}
#endif

#define INITERR_NOERROR 0
#define INITERR_NOPARAMS 1
#define INITERR_NOHBBSCOMMON 2
#define INITERR_INITCOMMONFAILED 3
#define INITERR_NOHBBSNODE 4
#define INITERR_INITDOORFAILED 5

static VOID CleanUp(ULONG num)
{
  if (HBBSNodeBase)
  {
    HBBS_CleanUpDoor();
    CloseLibrary (HBBSNodeBase);
  }

  if (HBBSCommonBase)
  {
    HBBS_CleanUpCommon();
    CloseLibrary (HBBSCommonBase);
  }

  switch(num)
  {
    case INITERR_NOERROR:
      exit(0);

    case INITERR_NOPARAMS:
      puts("Bad or no paramaters specified, check your config files\n"
           "(Don't try starting doors from the CLI!)");
      break;

    case INITERR_NOHBBSCOMMON:
      puts("Could not open HBBSCommon.library, check your assign's");
      break;

    case INITERR_INITCOMMONFAILED:
      puts("Could not initialise HBBSCommon.library functions\n"
           "Is HBBS Running ?");
      break;

    case INITERR_NOHBBSNODE:
      puts("Could not open HBBSNode.library, check your assign's");
      break;

    case INITERR_INITDOORFAILED:
      puts("The node specified is either not running or did not want to\n"
           "start this door now. (Don't run doors from the CLI!)");
      break;
  }
  exit(20);
}

static VOID Init(char *name)
{
  if(!(HBBSCommonBase = OpenLibrary("HBBSCommon.library",0)))
  {
    CleanUp(INITERR_NOHBBSCOMMON);
  }

  if (!(HBBS_InitCommon()))
  {
    CleanUp(INITERR_INITCOMMONFAILED);
  }

  if(!(HBBSNodeBase = OpenLibrary("HBBSNode.library",0)))
  {
    CleanUp(INITERR_NOHBBSNODE);
  }

  if (!(HBBS_InitDoor(N_NodeNum,name)))
  {
    CleanUp(INITERR_INITDOORFAILED);
  }
  SetProgramName(name);
}

/* **** DoorMain *********************************************************** */

void DoorMain( void )
{
  int loop;

  // Doors are still started even if the user has lost carrier so if you door prints anything
  // to the screen or displays any screens then you need to check to see if the user is still
  // online. to do this you must check N_ND->OnlineStatus (see below)

  // this might seem odd if you have programmed /X doors but doors in hbbs are really just
  // an extension of the main node program..

  // it is done like this so that you can write doors that continue to do stuff
  // without the user being around, for example, if a user hangs up during file checking
  // after an upload we wouldn't want all those nice files just stitting in
  // the playpen would we ?!

  // you can still use DOOR_SysopText() to display messages to the watch screen however...

  if (N_ND->OnlineStatus==OS_ONLINE)
  {
    for(loop=2;loop<gargc;loop++) // if there are any parameters then display them
    {
      sprintf(outstr,"Parameter %d = %s\n\r",loop-1,gargv[loop]);
      DOOR_WriteText(outstr);
    }
  }

}

int main(int argc,char **argv)
{

  /* set these so that we can access the paramaters in the rest of the program */

  gargc=argc;
  gargv=argv;

  /* Check to see if we've got any paramaters */

  if (argc <= 1)
  {
    CleanUp(INITERR_NOPARAMS);
  }

  /* Get the node number from the paramaters */

  if (sscanf(argv[1],"%d",&N_NodeNum)==0)
  {
    CleanUp(INITERR_NOPARAMS);
  }

  /* Initialise and set the door/program name */

  Init("<doorname>");

  /* if the init() didn't fail then we need to ask HBBS for the pointer to
     it's data structures */

  if (BBSGlobal=HBBS_GimmeBBS())
  {
    /* Then we need to get the node's data structures for the node this door is going to run on */

    if (N_ND=HBBS_NodeDataPtr(N_NodeNum))
    {

      /* Now that's done, you can put the rest of your code here, or in the DoorMain() function */

      DoorMain();
    }
  }

  /* Cleanup and close libraries */

  CleanUp(INITERR_NOERROR);
}

/* **** End Of File ******************************************************** */
