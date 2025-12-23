#include "/CINS_lib.h"

void read_config(void)
{
   BPTR  fp;

   char buff [ 256 ] = "";

   if((fp = Open(CINSCFG, MODE_OLDFILE)) && (Read(fp, &Config, sizeof(struct CINSCfg))))
      Close(fp);
   else
   {
      sprintf(buff, bm[635], CINSCFG);
      CNC_PutText(buff);
      GetOut();
   }

   Config.archie = NULL; /* must NULL these here to avoid problems on abort */
   Config.ftp    = NULL;
   Config.irc    = NULL;
   Config.telnet = NULL;

   if(fp = Open(CINSACC, MODE_OLDFILE))
   {
      Seek(fp, sizeof(struct CINSAccEntry) * z->user1.Access, OFFSET_BEGINNING);
      Read(fp, &UCfg.uae, sizeof(struct CINSAccEntry));
      Close(fp);
   }
} /* read_config */
