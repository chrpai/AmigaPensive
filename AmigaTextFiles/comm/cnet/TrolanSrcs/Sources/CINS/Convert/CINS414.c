/*
** CINS Configuration Update for CINS 4.14
** by Kelly Cochran
** Last Update: Mon Oct 20 23:49:27 1997
*/

#include "/CINS.h"

void main(void)
{
   struct CINSCfg      Config;
   struct CINSAccEntry ae;

   BPTR fp;

   memset(&ae,     0, sizeof(struct CINSAccEntry));
   memset(&Config, 0, sizeof(struct CINSCfg));

   if(fp = Open(CINSCFG, MODE_OLDFILE))
   {
      Read(fp, &Config, sizeof(struct CINSCfg));
      Close(fp);
   }
   else
   {
      Printf("Unable to open "CINSCFG"!\n");
      exit(5);
   }

   ae.FTPFree = Config.Free;
   ae.WebFree = Config.WebFree;
   ae.Rate    = Config.Rate;
   if(Config.CD)
      ae.Flags |= CINSAEF_CD;
   if(Config.UserUp)
      ae.Flags |= CINSAEF_UPLOADFTP;
   if(Config.WebUp)
      ae.Flags |= CINSAEF_UPLOADWEB;

   if(fp = Open(CINSACC, MODE_NEWFILE))
   {
      int i;
      for(i = 0; i < 32; i++)
      {
         Write(fp, &ae, sizeof(struct CINSAccEntry));
      }
      Close(fp);
   }
   else
   {
      Printf("Unable to open "CINSACC" for write!\n");
      exit(5);
   }
   Printf(CINSACC" created. Update complete\n");
}
