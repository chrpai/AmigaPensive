#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddIRCName(register __a1 struct List *list_a1, register __a2 struct IRCSite *ircs)
{
   struct List *list = list_a1;
   struct IRCEntry *irc;

   if(irc = (struct IRCEntry *)AllocVec(sizeof(struct IRCEntry), MEMF_CLEAR))
   {
      CopyMem(ircs, (APTR)&irc->irc_Site, sizeof(struct IRCSite));
      irc->irc_Node.ln_Name = irc->irc_Site.Site;
      AddTail((struct List *)list, (struct Node *)irc);
   }
}
