/*
** bbs.udata4 Compressor
** by Kelly Cochran
** Last Updated: Tue May 26 22:41:56 1998
*/

const UBYTE *version = "$VER: 1.1 UPacker "__AMIGADATE__;

void main(void)
{
   Forbid();
   if(FindPort(CNETPORT))
   {
      Permit();
      Printf("This MUST be run without CNet Control running!\n");
      return;
   }
   else
   {
      Permit();
      Printf("Checking for last valid account...\n");
      {
         BPTR fp;

         struct UserData udata;
         int curracc = 0,
             lastacc = 0;

         if(fp = Open("SysData:bbs.udata4", MODE_OLDFILE))
         {
            while(Read(fp, &udata, sizeof(struct UserData)))
            {
               if(*udata.Handle && (*udata.Handle != '!'))
               {
                  lastacc = curracc;
               }
               curracc++;
            }
            Close(fp);
            Printf("Last valid account is #%ld\n", ++lastacc);
         }
         else
         {
            Printf("Unable to open SysData:bbs.udata4!\n");
            return;
         }

         Printf("Now creating new bbs.udata4 file...\n");

         if(fp = Open("SysData:bbs.udata4", MODE_OLDFILE))
         {
            BPTR newfp;

            if(newfp = Open("SysData:bbs.udata4.new", MODE_NEWFILE))
            {
               while(lastacc)
               {
                  Read(fp, &udata, sizeof(struct UserData));
                  Write(newfp, &udata, sizeof(struct UserData));
                  Printf("%s\n", (*udata.Handle && *udata.Handle != '!') ? udata.Handle:"<empty>");
                  lastacc--;
               }
               Close(newfp);
            }
            else
            {
               Printf("Unable to make new bbs.udata4!\n");
               Close(fp);
               return;
            }
            Close(fp);
         }

         Printf("Saving old bbs.udata4 and moving in new bbs.udata4...\n");
         Rename("SysData:bbs.udata4", "SysData:bbs.udata4.old");
         Rename("SysData:bbs.udata4.new", "SysData:bbs.udata4");

         Printf("Now startup CNet and IMMEDIATELY run POINTERS\n");
      }
   }
}
