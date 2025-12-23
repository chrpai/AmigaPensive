#include "cins_protos.h"

void __saveds __asm CINS_FreeList(register __a1 struct List **list_a1)
{
   struct List **list = list_a1;
   struct Node *temp;
   struct Node *work;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         temp = work->ln_Succ;
         FreeVec(work);
         work = temp;
      }
      FreeVec(*list);

      *list = NULL;
   }
} /* free_list */
