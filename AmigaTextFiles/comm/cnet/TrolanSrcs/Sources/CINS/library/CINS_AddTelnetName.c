#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddTelnetName(register __a1 struct List *list_a1, register __a2 struct TelnetMSite *tnms)
{
   struct List *list = list_a1;
   struct TelnetMenu *tnm;

   if(tnm = (struct TelnetMenu *)AllocVec(sizeof(struct TelnetMenu), MEMF_CLEAR))
   {
      CopyMem(tnms, (APTR)&tnm->tnm_Site, sizeof(struct TelnetMSite));
      tnm->tnm_Node.ln_Name = tnm->tnm_Site.Menu;
      AddTail((struct List *)list, (struct Node *)tnm);
   }
}
