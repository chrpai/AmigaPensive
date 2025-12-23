/*
** CNet Uptime Shell command
** by Kelly Cochran
** Last updated: Wed Mar 11 12:12:34 1998
*/

enum
{
   ARG_PORTS = 0,
   NUM_ARGS
};

struct DosLibrary *DOSBase   = NULL;
struct ExecBase   *SysBase   = NULL;
struct Library    *CNet4Base = NULL;

void do_uptime(char *ports);

const char *version = "\0$VER: 1.3 CNetUptime "__AMIGADATE__;

int __saveds main(void)
{
   LONG args [ NUM_ARGS ];

   LONG rc = RETURN_FAIL;

   struct RDArgs *rda;

   SysBase = *(struct ExecBase **)4L;

   DOSBase = (struct DosLibrary *)OpenLibrary(DOSNAME, 37L);

   if(!DOSBase)
      return (rc);

   memset(&args, 0, sizeof(args));

   if(rda = ReadArgs("PORTS", (LONG *)&args, NULL))
   {
      do_uptime((char *)args[ARG_PORTS]);

      rc = RETURN_OK;

      FreeArgs(rda);
   }

   CloseLibrary((struct Library *)DOSBase);

   return (rc);
}

void do_uptime(char *port)
{
   struct MainPort *myp;

   if(CNet4Base = OpenLibrary("cnet4.library", 4L))
   {
      if(myp = (struct MainPort *)FindPort("cnetport"))
      {
         struct RangeContext rc;

         int i;

         LONG now = CNetTime();

         char outbuff [ 1024 ] = "";

         Printf("Port Uptimes\n");

         CNetUpTime(outbuff, now - CNetImplodeTime(&myp->SAMDate[1]));
         Printf("Control: %s\n", outbuff);

         if(port)
         {
            if(*port == '*')
               port = "0-";

            if(CNetFindRange(port, 0, myp->HiPort, &rc))
            {
               while((i = CNetNextRange(&rc)) >= 0)
               {
                  if(myp->pc[i].online)
                  {
                     CNetUpTime(outbuff, now - CNetImplodeTime(&myp->PortZ[i]->BootDate));
                     Printf("Port %2ld: %s\n", i, outbuff);
                  }
               }
            }
         }
      }
      else
      {
         Printf("Unable to find CONTROL\n");
      }

      CloseLibrary(CNet4Base);
   }
}
