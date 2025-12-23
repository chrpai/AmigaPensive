/*
** Conversion utility to Zero out unused sections of CINS.cfg
** by Kelly Cochran
** Last updated: Sun Dec 14 01:51:09 1997
*/

#include "/CINS.h"

void main(void)
{
   struct CINSCfg Config;

   BPTR fp;

   Printf("Loading old CINS.cfg..\n");

   if(fp = Open(CINSCFG, MODE_OLDFILE))
   {
      Read(fp, &Config, sizeof(struct CINSCfg));
      Printf("Cleaning out unused data segments...");
      Flush(Output());

      memset(&Config.def_ftp, 0, sizeof(struct NcFTPCfg));

      Config.Serial      = 0L;
      Config.def_Shell   = 0;
      Config.def_Muff    = 0;
      Config.def_DirANSI = 0;
      Config.Version     = 4;
      Config.obs_WebFree = 0L;

      Printf("done\n");

      Seek(fp, 0, OFFSET_BEGINNING);
      Write(fp, &Config, sizeof(struct CINSCfg));
      Close(fp);

      Printf("Update to version %ld configuration complete.\n", Config.Version);
   }
   else
   {
      Printf("Unable to open "CINSCFG"!\n");
   }
}
