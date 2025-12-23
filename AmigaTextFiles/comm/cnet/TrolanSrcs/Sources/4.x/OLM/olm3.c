/*
** OLM Sending Program for CNet/4
** by Kelly Cochran
** Last updated: Tue Nov 18 23:51:18 1997
*/

#include <stdio.h>
#include <stdlib.h>

struct MainPort   *myp       = NULL;

const char *version = "\0$VER: OLM/3 1.2 "__AMIGADATE__;

BOOL FakeOLM(char  *ByUser,
             char  *Message,
             short ByAccount,
             short Port,
             short ToPort,
             BOOL  BCast
             );

void main(void)
{
   struct RDArgs __aligned  *rda;

   struct args
   {
      ULONG from;
      ULONG fromport;
      ULONG toport;
      ULONG message;
   };

   struct args myargs;

	LONG  ByAcc    = 0,
         fromport = 0;

   BOOL  BCast    = FALSE;

   char  *from    = NULL,
         *message,
         *toport  = NULL;

   int   i;

   memset(&myargs, 0, sizeof(struct args));

   Forbid();
   if(myp = (struct MainPort *)FindPort("cnetport"))
   {
      Permit();
      if(rda = ReadArgs("F=FROM/K,FP=FROMPORT/K,TOPORT/A,MESSAGE/A/F",
                         (LONG *)&myargs, NULL))
      {
         if(myargs.from)
            from     = (char *)myargs.from;
         if(myargs.fromport)
            fromport = atoi((char *)myargs.fromport);
         if(myargs.toport)
         {
            toport = (char *)myargs.toport;
            if(*toport == '*')
               BCast = TRUE;
         }
         if(myargs.message)
         {
            message = (char *)myargs.message;

            if(from && *from && myargs.fromport)
            {
               ByAcc = 1;
            }

            if(BCast)
            {
               for(i = 0; i < myp->HiPort; i++)
                  FakeOLM(from, message, ByAcc, fromport, i, BCast);
            }
            else
            {
               if(i = atoi((char *)toport))
                  FakeOLM(from ? from : NULL, message, ByAcc, myargs.fromport ? fromport : NULL, i, BCast);
            }
         }
         else
            Printf("No message specified!\n");

         FreeArgs(rda);
      }
      else
            Printf("Invalid arguments.\n");
   }
   else
   {
      Permit();
      Printf("Could not find CONTROL process\n\n");
   }

   exit(0);
}

BOOL FakeOLM(char  *ByUser,
             char  *Message,
             short ByAccount,
             short Port,
             short ToPort,
             BOOL  BCast
             )
{
   struct OLMHeader OL_Head;
   BPTR   OL_FP;
   char   OL_File [ 64 ];

   if(myp->PortZ[ToPort]->Carrier)
   {
      memset(&OL_Head, 0, sizeof(struct OLMHeader));

      if(ByAccount)
      {
         strcpy(OL_Head.ByUser, ByUser);

         OL_Head.ByAccount = ByAccount;
         OL_Head.Port      = Port;
      }

      if(BCast)
         OL_Head.broadcast = TRUE;

      sprintf(OL_File, "%s_olm%d", myp->gc.OLMpath, ToPort);

      if(OL_FP = OpenAppend(OL_File))
      {
         Write(OL_FP, &OL_Head, sizeof(struct OLMHeader));
         FPrintf(OL_FP, "%s\x0A\x1A\x0A", Message);
         Close(OL_FP);
      }

      myp->PortZ[ToPort]->OLMWaiting++;

      return TRUE;
   }

   return FALSE;
}
