#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddTelnetDataName(register __a1 struct List *list_a1, register __a2 struct TelnetSite *tns)
{
   struct List *list = list_a1;
   struct TelnetEntry *tn;

   if(tn = (struct TelnetEntry *)AllocVec(sizeof(struct TelnetEntry), MEMF_CLEAR))
   {
      CopyMem(tns, (APTR)&tn->tn_Site, sizeof(struct TelnetSite));
      tn->tn_Node.ln_Name = tn->tn_Site.Comm1;
      AddTail((struct List *)list, (struct Node *)tn);
   }
}
