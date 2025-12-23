#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertIRCName(register __a1 struct List *list_a1, register __a2 struct IRCSite *ircs, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct IRCEntry *irc,
                   *work = (struct IRCEntry *)CINS_FindOrdinal(list, pos);

   if(irc = (struct IRCEntry *)AllocVec(sizeof(struct IRCEntry), MEMF_CLEAR))
   {
      CopyMem(ircs, (APTR)&irc->irc_Site, sizeof(struct IRCSite));
      irc->irc_Node.ln_Name = irc->irc_Site.Site;
      Insert((struct List *)list, (struct Node *)irc, (struct Node *)work->irc_Node.ln_Pred);
   }
}
