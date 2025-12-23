#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertArchieName(register __a1 struct List *list_a1, register __a2 struct ArchieSite *arcs, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct ArchieEntry *arc,
                      *work = (struct ArchieEntry *)CINS_FindOrdinal(list, pos);

   if(arc = (struct ArchieEntry *)AllocVec(sizeof(struct ArchieEntry), MEMF_CLEAR))
   {
      CopyMem(arcs, (APTR)&arc->arc_Site, sizeof(struct ArchieSite));
      arc->arc_Node.ln_Name = arc->arc_Site.Site;
      Insert((struct List *)list, (struct Node *)arc, (struct Node *)work->arc_Node.ln_Pred);
   }
}
