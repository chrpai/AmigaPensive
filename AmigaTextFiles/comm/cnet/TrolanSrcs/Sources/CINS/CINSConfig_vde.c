/*
** CINS Configuration VDE component
** by Kelly Cochran
** Last updated: Fri Mar 26 13:35:37 1999
*/

#ifndef CINSCONFIG_VDE_C   /* VDE safety */
#define CINSCONFIG_VDE_C
#endif

#include "CINS.h"
#include "CINSConfig.h"
#include "CINSVDE.h"

void VDE_main(void)
{
   long tmp;

   Config.Serial = check_keyfile();

   if(Config.Serial == -1)
   {
      strcpy(CCfg[ 28 ].text, " c9Unregistered DEMOTO ");
      CCfg[ 28 ].ghost = (1<<0);
   }

   if(make_config_VDE())
   {
      tmp = (long)z->uz[0]; /* Save old z->uz[0] */

      z->uz[0] = TRUE;     /* Ghost for Expansion entries */

      if(CNC_VisualDataEditor("CINS/CINS", &Config, sizeof(struct CINSCfg)))
         save_config();

      z->uz[0] = tmp;      /* Restore old z->uz[0] */
   }
}

BOOL make_config_VDE(void)
{
   BPTR  fp,
         lock;

   int i;

   char **bm = myp->FirstTT->bm;

   for(i = 2000; bm[i] && *bm[i]; i++)
   {
      if(strstr(bm[i], "CINS Stuff}"))
      {
         Config.Line = i+1;
         break;
      }
   }

   CCfg[ 14 ].min = Config.Line;
   CCfg[ 17 ].min = Config.Line + 5;

   if(lock = CreateDir("SysText:VDE/CINS"))
      UnLock(lock);
   if(fp = Open("SysText:VDE/CINS/CINS", MODE_NEWFILE))
   {
      FWrite(fp, (char*)CCfg, sizeof(struct VDEentry), VDE_SIZE(CCfg));
      Close(fp);
   }
   else
      return FALSE;
   return TRUE;
}
