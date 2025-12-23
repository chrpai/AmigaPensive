/*  This is a Door Coder's debugging tool */


#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>

#include <dos/dosextens.h>
#include <intuition/screens.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <diskfont/diskfont.h>
#include <utility/utility.h>
#include <graphics/gfxbase.h>
#include <workbench/workbench.h>
#include <graphics/scale.h>
#include <clib/wb_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <clib/diskfont_protos.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#ifdef __SASC
int CXBRK(void) { return(0); }
int _CXBRK(void) { return(0); }
void chkabort(void) {}
#endif

#include <HBBS/ANSI_Codes.h>
#include <HBBS/Defines.h>
#include <HBBS/types.h>
#include <HBBS/structures.h>
#include <HBBS/hbbscommon_protos.h>

#ifdef __SASC
#include <HBBS/hbbscommon_pragmas_sas.h>
#else
#include <HBBS/hbbscommon_pragmas.h>
#endif

#include <HBBS/Hbbsnode_protos.h>

#ifdef __SASC
#include <HBBS/Hbbsnode_pragmas_sas.h>
#else
#include <HBBS/Hbbsnode_pragmas.h>
#endif

struct Library *HBBSCommonBase=NULL;
struct Library *HBBSNodeBase=NULL;

struct BBSGlobalData *BBSGlobal=NULL;
int N_NodeNum=-1;

static VOID cleanup(ULONG num)
{
  if (HBBSCommonBase)
  {
    HBBS_CleanUpCommon();
    CloseLibrary (HBBSCommonBase);
  }

  if (num) printf("Door Error = %d\n",num);

  exit(0);
}

static VOID init(char *name)
{
  if(!(HBBSCommonBase = OpenLibrary("HBBSCommon.library",0)))
  {
    cleanup(1);
  }

  if (!(HBBS_InitCommon()))
  {
    cleanup(2);
  }

  SetProgramName(name);
}


void DoorMain(int argc,char *argv[])
{
  struct ConfData *conf;
  struct Node *node;
  int loop,pathcounter;
  struct NodeData *nodedata=NULL;

  /* 
   * you can now access any HBBS data structure you like, for instance...
   */

  printf("Config Log is %s, filename of log is: %s\n", BBSGlobal->LogConfig ? "ON" : "OFF",BBSGlobal->ConfigLogFile);


  for (nodedata=HBBS_NodeDataPtr(0) ; nodedata->node.ln_Succ ; nodedata = (struct NodeData *) nodedata->node.ln_Succ)
  {

      /*
       * you must do this before you attempt to access *any* other data about
       * the node!  All data is considered INVALID unless the node is
       * online or at the await connect screen.
       */
    
    printf("Node %d information :\n",nodedata->NodeNum);
    
    if ((nodedata->Status == STAT_READY) || (nodedata->Status == STAT_ONLINE))
    { 
    
        /*
         * Display some informaton
         */

      if (nodedata->KeyOK) puts("The sysop appears to have registered his copy of HydraBBS\n"
                            "but it's probably cracked...");

        /*
         * If there is a user online, and it is a valid user (i.e. they've logged on)
         * then display some info about the user!
         */

      if (nodedata->User->Valid)
      {
        printf("Currently logged on user has written %ld message(s)\n",nodedata->User->MsgsWritten);
        printf("Access: C=%d N=%d\n",nodedata->User->CallData->Access,nodedata->User->NormalData->Access);
      }
      else
      {
        printf("No-one logged on.\n");
      }

        /*
         * Display some info about the serial device
         */
      
      if (nodedata->NodeSettings.UseDevice)
      {
        printf("Hardware: %s Amiga .device name: %s\n",nodedata->NodeDevice.DeviceName,nodedata->NodeDevice.SerialDevice);
        if (nodedata->SerOK) puts("Serial port OK"); else puts("Serial port NOT OK!");
        if (nodedata->SerWaiting) puts("Serial waiting"); else puts("Serial not waiting!");
        if (nodedata->NodeFlags & NFLG_BLOCKSERIAL) puts("Serial input blocked!"); else puts("Serial input not blocked");
      }
      else
      {
        puts("The node is not configured to use a serial device!");
      }

      if (nodedata->ConOK) puts("Console port OK"); else puts("Console port NOT OK!");
      if (nodedata->ConWaiting) puts("Console waiting"); else puts("Console not waiting!");

    }
    else
    {
      printf("Node %d is not started!\n",nodedata->NodeNum);
    }
  }


    /*
     * Print amount of conferences, conference names and upload/download paths
     */

  printf("There are %ld conferences\n",BBSGlobal->Conferences);

  if (BBSGlobal->Conferences > 0 ) // just to be on the safe side!
  {
    
      /* 
       * process each conference, and increment a counter too..
       */
         
    for (conf = (struct ConfData *) BBSGlobal->ConfList->lh_Head , loop = 0 ;
         conf -> node.ln_Succ ;
         conf = (struct ConfData *) conf->node.ln_Succ , loop ++ )
    {  
        /*
         *  Display info.
         */              
         
      printf("Conference %d - %s\n", loop+1, conf->node.ln_Name);

      if (conf->Upload)
      {
        for (pathcounter = 1, node = conf->Upload->lh_Head ; node->ln_Succ ; node = node->ln_Succ )
        {
          printf("Upload Path %d = %s\n", pathcounter++, node->ln_Name);
        }
      }
      else puts("There are no upload paths!");

      if (conf->Download)
      {
        for (pathcounter = 1, node = conf->Download->lh_Head ; node->ln_Succ ; node = node->ln_Succ )
        {
          printf("Download Path %d = %s\n", pathcounter++, node->ln_Name);
        }
      }
      else puts("There are no download paths!");


    }
  }
}

int main(int argc,char *argv[])
{
  init("PrintData");

  if (BBSGlobal=HBBS_GimmeBBS())
  {
    DoorMain(argc,argv);
  }
  else
  {
    puts("Could not interface to HBBS!");
  }
  cleanup(0);
}
