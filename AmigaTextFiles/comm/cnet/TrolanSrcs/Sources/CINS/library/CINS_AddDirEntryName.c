#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddDirEntryName(register __a1 struct List *list_a1, register __a2 struct DirEntryNode *den)
{
   struct List *list = list_a1;
   struct DirEntryList *del;

   if(del = (struct DirEntryList *)AllocVec(sizeof(struct DirEntryList), MEMF_CLEAR))
   {
      CopyMem(den, (APTR)&del->del_DEN, sizeof(struct DirEntryNode));
      del->del_Node.ln_Name = del->del_DEN.FileName;
      AddTail((struct List *)list, (struct Node *)del);
   }
}
