#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddFTPName(register __a1 struct List *list_a1, register __a2 struct FTPMSite *ftps)
{
   struct List *list = list_a1;
   struct FTPMenu *ftpm;

   if(ftpm = (struct FTPMenu *)AllocVec(sizeof(struct FTPMenu), MEMF_CLEAR))
   {
      CopyMem(ftps, (APTR)&ftpm->ftpm_Site, sizeof(struct FTPMSite));
      ftpm->ftpm_Node.ln_Name = ftpm->ftpm_Site.Menu;
      AddTail((struct List *)list, (struct Node *)ftpm);
   }
}
