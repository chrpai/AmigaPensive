#include "cnet3lib.h"

struct List *init_list(void)
{
   struct List *list;

   if(list = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
      NewList(list);

   return (list);
}
