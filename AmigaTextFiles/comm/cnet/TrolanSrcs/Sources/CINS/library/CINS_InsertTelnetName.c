#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertTelnetName(register __a1 struct List *list_a1, register __a2 struct TelnetMSite *tnms, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct TelnetMenu *tnm,
                     *work = (struct TelnetMenu *)CINS_FindOrdinal(list, pos);

   if(tnm = (struct TelnetMenu *)AllocVec(sizeof(struct TelnetMenu), MEMF_CLEAR))
   {
      CopyMem(tnms, (APTR)&tnm->tnm_Site, sizeof(struct TelnetMSite));
      tnm->tnm_Node.ln_Name = tnm->tnm_Site.Menu;
      Insert((struct List *)list, (struct Node *)tnm, (struct Node *)work->tnm_Node.ln_Pred);
   }
}
