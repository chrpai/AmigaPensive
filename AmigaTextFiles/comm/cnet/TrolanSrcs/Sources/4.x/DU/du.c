/*
** El Cheapo Dump shell command for CNet/4
** by Kelly Cochran
** Last updated: Wed Aug 27 17:02:32 1997
*/

const char *version = "\0$VER: 1.1 Dump "__AMIGADATE__;

void main(void)
{
   struct RDArgs __aligned *myargs;
   struct MainPort         *myp;
   struct Library          *CNetBase;
   struct Library          *CNet4Base;
   struct RangeContext     rc;

   LONG args [3] = { NULL,
                     NULL,
                     NULL };

   char *ports;
   BOOL allports = FALSE;
   BOOL close    = FALSE;
   int  dumptime = 0;
   int  i;

   Forbid();
   if(myp = (struct MainPort *)FindPort("cnetport"))
   {
      Permit();
      if(myp->ClickMaint)
      {
         if(CNetBase = OpenLibrary("cnet.library", 4))
         {
            if(CNet4Base = OpenLibrary("cnet4.library", 4))
            {
               if(myargs = ReadArgs("PORT/A,TIME/A,CLOSE/S", args, NULL))
               {
                  if(args[0])
                  {
                     ports = (char *)args[0];
                     if(*ports == '*')
                     {
                        allports = TRUE;
                     }
                     if(args[1])
                     {
                        dumptime = atoi((char *)args [ 1 ]);
                     }
                     if(args[2])
                     {
                        close = TRUE;
                     }

                     if(CNetFindRange(allports ? "0-" : ports, 0, myp->HiPort, &rc))
                     {
                        while((i = CNetNextRange(&rc)) >= 0)
                        {
                           if(myp->pc[i].online)
                           {
                              myp->PortZ[i]->TimeLeft = dumptime;
                              if(close)
                              {
                                 myp->PortZ[i]->getout = TRUE;
                              }
                           }
                        }

                        if(allports)
                        {
                           Printf("All ports set to logoff %sin %ld minutes\n", close ? "and close ":"", dumptime);
                        }
                        else
                        {
                           Printf("Ports %s set to logoff %sin %ld minutes\n", ports, close ? "and close ":"", dumptime);
                        }
                     }
                  }
                  else
                  {
                     Printf("You must specify a port or range of ports to DU.\n");
                  }

                  FreeArgs(myargs);
               }
               CloseLibrary(CNet4Base);
            }
            else
            {
               Printf("Could not open cnet4.library!\n");
            }
            CloseLibrary(CNetBase);
         }
         else
         {
            Printf("Could not open cnet.library!\n");
         }
      }
      else
      {
         Printf("Unlock control you DORK!\n");
      }
   }
   else
   {
      Permit();
   }
}
