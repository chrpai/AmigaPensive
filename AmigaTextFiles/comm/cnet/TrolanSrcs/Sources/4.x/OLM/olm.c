/*
** OLM Sending Program for CNet/4
** by Kelly Cochran
** Last updated: Wed Apr 29 22:56:13 1998
*/

struct MainPort   *myp       = NULL;
struct Library    *CNet4Base = NULL;
struct ExecBase   *SysBase   = NULL;
struct DosLibrary *DOSBase   = NULL;

const char *version = "\0$VER: OLM 1.3 "__AMIGADATE__;

int __saveds main(void);
void my_sprintf(char *buff, char *fmt, ...);
int my_atoi(char *value);
void my_memset(void *start, int value, int size);
BOOL FakeOLM(ULONG ByID,
             char  *ByUser,
             char  *Message,
             short ByAccount,
             short Port,
             short ToPort,
             BOOL  BCast
             );

int __saveds main(void)
{
   struct RDArgs __aligned  *rda;
   struct RangeContext       rc;

   struct args
   {
      ULONG from;
      ULONG fromport;
      ULONG toport;
      ULONG message;
   };

   struct args myargs;

	LONG  ByAcc    = 0,
         fromport = 0,
         ID       = 0;

   BOOL  BCast    = FALSE;

   char  *from    = NULL,
         *message,
         *toport  = NULL;

   int   i;

   SysBase = *(struct ExecBase **)4L;

   if(DOSBase = (struct DosLibrary *)OpenLibrary(DOSNAME, 37L))
   {
      my_memset(&myargs, 0, sizeof(struct args));

      Forbid();
      if(myp = (struct MainPort *)FindPort("cnetport"))
      {
         Permit();
         if(CNet4Base = OpenLibrary("cnet4.library", 4))
         {
            if(rda = ReadArgs("F=FROM/K,FP=FROMPORT/K,TOPORT/A,MESSAGE/A/F",
                               (LONG *)&myargs, NULL))
            {
               if(myargs.from)
                  from     = (char *)myargs.from;
               if(myargs.fromport)
                  fromport = my_atoi((char *)myargs.fromport);
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
                     ID    = 0xF00DF00D;
                  }

                  if(CNetFindRange(BCast ? "0-" : toport, 0, myp->HiPort, &rc))
                  {
                     while((i = CNetNextRange(&rc)) >= 0)
                        FakeOLM(ID, from, message, ByAcc, fromport, i, BCast);
                  }
                  else
                  {
                     if(i = my_atoi((char *)toport))
                        FakeOLM(ID, from ? from : NULL, message, ByAcc, myargs.fromport ? fromport : NULL, i, BCast);
                  }
               }
               else
                  Printf("No message specified!\n");

               FreeArgs(rda);
            }
            else
               Printf("Invalid arguments.\n");
            CloseLibrary(CNet4Base);
         }
         else
            Printf("Could not open cnet4.library.\n");
      }
      else
      {
         Permit();
         Printf("Could not find CONTROL process\n\n");
      }
      CloseLibrary((struct Library *)DOSBase);
   }

   return 0;
}

BOOL FakeOLM(ULONG ByID,
             char  *ByUser,
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
      my_memset(&OL_Head, 0, sizeof(struct OLMHeader));

      if(ByID)
      {
         my_sprintf(OL_Head.ByUser, "%s", ByUser);

         OL_Head.ByID      = ByID;
         OL_Head.ByAccount = ByAccount;
         OL_Head.Port      = Port;
      }

      OL_Head.date = CNetTime();

      if(BCast)
         OL_Head.broadcast = TRUE;

      my_sprintf(OL_File, "%s_olm%d", myp->gc.OLMpath, ToPort);

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

UWORD fmtfunc[] = { 0x16c0, 0x4e75 };
void my_sprintf(char *buff, char *fmt, ...)
{
   RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, buff );
}

int my_atoi(char *value)
{
   int val = 0;

   char *tmp = value;

   while(tmp && *tmp)
   {
      val *= 10;
      val += *tmp - '0';
      tmp++;
   }

   return val;
}

void my_memset(void *start, int value, int size)
{
   char *tmp = start;

   while(size)
   {
      *tmp = value;
      tmp++;
      size--;
   }
}

BPTR OpenAppend(char *file)
{
   BPTR fp;

   if(fp = Open(file, MODE_OLDFILE))
   {
      Seek(fp, 0, OFFSET_END);
      return fp;
   }
   else if(fp = Open(file, MODE_NEWFILE))
   {
      return fp;
   }
   else
      return NULL;
}
