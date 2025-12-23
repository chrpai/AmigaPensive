#include "cnet3lib.h"

struct Node *FindOrdinal(struct List *list, int node)
{
   struct Node *tmp;

   tmp = list->lh_Head;

   while(node && (tmp = tmp->ln_Succ))
      node--;

   return (tmp);
}
