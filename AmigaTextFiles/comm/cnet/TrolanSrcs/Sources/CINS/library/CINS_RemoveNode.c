#include "cins_protos.h"

BOOL __saveds __asm CINS_RemoveNode(register __a1 struct List *list_a1, register __d1 int node_d1)
{
   struct List *list = list_a1;
   struct Node *work;
   int node = node_d1;

   if(list)
   {
      work = CINS_FindOrdinal(list, node);

      if(work)
      {
         Remove(work);
         FreeVec(work);
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }

   return FALSE;
}
