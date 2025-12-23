/*
** Subboards Cleanup Utility
** by Kelly Cochran
** Last updated: Fri Apr 17 21:04:26 1998
*/

int __saveds main(void);
void packsub4(void);

static char *version = "\0$VER: Sub4Wipe 1.1 "__AMIGADATE__;

struct ExecBase   *SysBase;
struct DosLibrary *DOSBase;

int __saveds main(void)
{
   SysBase = *(struct ExecBase **)4L;

   if(DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37L))
   {
/*
      if(FindPort("cnetport"))
      {
         Printf("CONTROL must be shutdown prior to running this utility\n");
         return RETURN_FAIL;
      }
*/
      packsub4();

      CloseLibrary((struct Library *)DOSBase);

      return RETURN_OK;
   }

   return RETURN_FAIL;
}

void packsub4(void)
{
   BPTR sub4fp,
        sub4tfp;

   if(sub4fp = Open("SysData:subboards4", MODE_OLDFILE))
   {
      struct SubboardType4 sb4,
                           sb4tmp;

      memset(&sb4   , 0, sizeof(struct SubboardType4));
      memset(&sb4tmp, 0, sizeof(struct SubboardType4));

      sb4tmp.Marker |= MRK_SUBBOARD_KILLED;

      if(sub4tfp = Open("SysData:subboards4.new", MODE_NEWFILE))
      {
         while(Read(sub4fp, &sb4, sizeof(struct SubboardType4)))
         {
            Write(sub4tfp, (sb4.Marker & MRK_SUBBOARD_KILLED) ? &sb4tmp:&sb4, sizeof(struct SubboardType4));
            if(sb4.Marker & MRK_SUBBOARD_KILLED)
            {
               Printf("Wiping up %s\n", sb4.Title);
            }
         }
         Close(sub4tfp);
      }
      Close(sub4fp);
   }
}
