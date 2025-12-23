#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertFTPName(register __a1 struct List *list_a1, register __a2 struct FTPMSite *ftps, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct FTPMenu *ftpm,
                  *work = (struct FTPMenu *)CINS_FindOrdinal(list, pos);

   if(ftpm = (struct FTPMenu *)AllocVec(sizeof(struct FTPMenu), MEMF_CLEAR))
   {
      CopyMem(ftps, (APTR)&ftpm->ftpm_Site, sizeof(struct FTPMSite));
      ftpm->ftpm_Node.ln_Name = ftpm->ftpm_Site.Menu;
      Insert((struct List *)list, (struct Node *)ftpm, (struct Node *)work->ftpm_Node.ln_Pred);
   }
}
