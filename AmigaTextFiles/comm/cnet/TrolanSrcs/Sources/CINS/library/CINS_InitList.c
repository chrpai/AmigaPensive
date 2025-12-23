#include "cins_protos.h"

struct List * __saveds __asm CINS_InitList(void)
{
   struct List *list;

   if(list = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(list);
   }

   return (list);
}
