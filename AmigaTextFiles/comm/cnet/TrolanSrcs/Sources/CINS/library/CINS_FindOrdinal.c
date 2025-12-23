#include "cins_protos.h"

struct Node * __saveds __asm CINS_FindOrdinal(register __a1 struct List *list_a1, register __d1 int node_d1)
{
   struct List *list = list_a1;
   struct Node *tmp;
   int node = node_d1;

   tmp = list->lh_Head;

   while(node && (tmp = tmp->ln_Succ))
      node--;

   return (tmp);
}
