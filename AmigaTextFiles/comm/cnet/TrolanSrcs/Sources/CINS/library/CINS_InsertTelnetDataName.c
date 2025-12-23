#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertTelnetDataName(register __a1 struct List *list_a1, register __a2 struct TelnetSite *tns, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct TelnetEntry *tn,
                      *work = (struct TelnetEntry *)CINS_FindOrdinal(list, pos);

   if(tn = (struct TelnetEntry *)AllocVec(sizeof(struct TelnetEntry), MEMF_CLEAR))
   {
      CopyMem(tns, (APTR)&tn->tn_Site, sizeof(struct TelnetSite));
      tn->tn_Node.ln_Name = tn->tn_Site.Comm1;
      Insert((struct List *)list, (struct Node *)tn, (struct Node *)work->tn_Node.ln_Pred);
   }
}
