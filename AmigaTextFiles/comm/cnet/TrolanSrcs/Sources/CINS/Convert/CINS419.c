/*
** Conversion utility to Migrate purge days and grow CINS.acc
** by Kelly Cochran
** Last updated: Mon Dec 29 02:21:33 1997
*/

#include "/CINS.h"

void main(void)
{
   struct CINSCfg Config;

   BPTR fp;

   Printf("Loading old CINS.cfg..\n");

   if(fp = Open(CINSCFG, MODE_OLDFILE))
   {
      BPTR fp2,
           fp3;

      struct CINSAccEntry ae;

      Read(fp, &Config, sizeof(struct CINSCfg));

      Close(fp);

      Printf("Updating "CINSACC"...");
      Flush(Output());

      if((fp2 = Open(CINSACC, MODE_OLDFILE)) && (fp3 = Open("CINS:Cfg/CINS.acc2", MODE_NEWFILE)))
      {
         memset(&ae, 0, sizeof(struct CINSAccEntry));

         while(Read(fp2, &ae, sizeof(struct OldCINSAccEntry)))
         {
            ae.PurgeDays = Config.Days;
            Write(fp3, &ae, sizeof(struct CINSAccEntry));
         }

         Close(fp2);
         Close(fp3);

         Rename(CINSACC, "CINS:Cfg/CINS.acc.old");
         Rename("CINS:Cfg/CINS.acc2", CINSACC);

         Printf("done!\n");
      }
   }
   else
   {
      Printf("Unable to open "CINSCFG"!\n");
   }
}
