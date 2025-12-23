#include "cnet4lib.h"

void free_list(struct List **list)
{
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
