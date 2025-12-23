#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddArchieName(register __a1 struct List *list_a1, register __a2 struct ArchieSite *arcs)
{
   struct List *list = list_a1;
   struct ArchieEntry *arc;

   if(arc = (struct ArchieEntry *)AllocVec(sizeof(struct ArchieEntry), MEMF_CLEAR))
   {
      CopyMem(arcs, (APTR)&arc->arc_Site, sizeof(struct ArchieSite));
      arc->arc_Node.ln_Name = arc->arc_Site.Site;
      AddTail((struct List *)list, (struct Node *)arc);
   }
}
