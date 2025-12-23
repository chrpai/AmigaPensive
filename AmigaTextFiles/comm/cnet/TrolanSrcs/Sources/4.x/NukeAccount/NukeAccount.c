/*
** CNet/4 Account 'Nuker'
** by Kelly Cochran
** Last update: Mon May 04 23:09:41 1998
*/

enum
{
   ACCOUNT = 0,
   NUM_ARGS
};

const char *version = "\0$VER: 1.2 NukeAccount\0";

struct MainPort   *myp      = NULL;
struct Library    *CNetBase = NULL;
struct ExecBase   *SysBase;
struct DosLibrary *DOSBase  = NULL;

int __saveds main(void)
{
   ULONG args [ NUM_ARGS ];

   struct RDArgs *rda;

   SysBase = *(struct ExecBase **)4L;

   if(DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37L))
   {
      Forbid();
      myp = (struct MainPort *)FindPort(CNETPORT);
      Permit();

      if(myp)
      {
         if(rda = ReadArgs("ACCOUNT/A", (LONG *)args, NULL))
         {
            int acc = atoi((char *)args[ACCOUNT]);

            if(acc)
            {
               if(CNetBase = OpenLibrary("cnet.library", 4L))
               {
                  struct UserData *udata = LockAccount(acc);

                  if(udata)
                  {
                     memset(udata, 0, sizeof(struct UserData));

                     udata->Handle[0] = '!';

                     UnLockAccount(acc, TRUE);

                     Printf("Account %ld toastied\n", acc);
                     Printf("Now run pointers!\n");
                  }
                  else
                  {
                     Printf("Unable to locate account %ld!\n", acc);
                  }
                  CloseLibrary(CNetBase);
               }
            }
            FreeArgs(rda);
         }
      }
      CloseLibrary((struct Library *)DOSBase);
   }
}
