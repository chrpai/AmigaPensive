/*
** CNet/4 Command Snapshot Tool
** by Kelly Cochran
** Last updated: Wed Sep 24 01:47:43 1997
*/

#define CMD_TEMPLATE "PORTS/A,HISTORY/N"

const char *version = "\0$VER: Snapshot 1.0 "__AMIGADATE__;

void main(void)
{
   struct MainPort         *myp;
   struct Library          *CNet4Base;
   struct RDArgs __aligned *rda;
   struct RangeContext      rc;

   struct args
   {
      ULONG ports;
      ULONG history;
   };

   struct args myargs;

   char *ports   = NULL;
   long  history = 0L;

   BOOL allports = FALSE;

   int i,
       j;

   memset(&myargs, 0, sizeof(struct args));

   Forbid();
   if(myp = (struct MainPort *)FindPort(CNETPORT))
   {
      Permit();
      if(CNet4Base = OpenLibrary("cnet4.library", 4))
      {
         if(rda = ReadArgs(CMD_TEMPLATE, (LONG *)&args, NULL))
         {
            if(args.ports)
            {
               ports = (char *)args.ports;
               if(*ports == '*')
               {
                  allports = TRUE;
               }
            }
            if(args.history)
            {
               history = *(long *)args.history;
               if(history > 10)
                  history = 10;
               if(history < 0)
                  history = 0;
            }

            if(CNetFindRange(allports ? "0-":ports, 0, myp->HiPort, &rc))
            {
               while((i = CNetNextRange(&rc)) >= 0)
               {
                  if(myp->pc[i].online)
                  {
                     Printf("Port %ld\n", i);
                     if(*myp->PortZ[i]->InBuffer)
                     {
                        Printf("\tInBuffer: %s\n", myp->PortZ[i]->InBuffer);
                     }
                     if(history)
                     {
                        for(j = 0; j < history; j++)
                        {
                           if(*myp->PortZ[i]->BackBuff[j])
                           {
                              Printf("\tBackBuff[%ld]: %s\n", j, myp->PortZ[i]->BackBuff[j]);
                           }
                        }
                     }
                  }
               }
            }

            FreeArgs(rda);
         }
         CloseLibrary(CNet4Base);
      }
      else
      {
         Printf("Could not open cnet4.library\n");
      }
   }
   else
   {
      Permit();
      Printf("Could not find CONTROL process\n");
   }
}
