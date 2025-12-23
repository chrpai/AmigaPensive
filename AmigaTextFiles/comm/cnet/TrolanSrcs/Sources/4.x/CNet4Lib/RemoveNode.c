#include "cnet4lib.h"

BOOL RemoveNode(struct List *list, int node)
{
   struct Node *work;

   if(list)
   {
      work = FindOrdinal(list, node);

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
