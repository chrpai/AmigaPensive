#include "cins_protos.h"
#include "/CINS.h"

void __saveds __asm CINS_AddFTPDataName(register __a1 struct List *list_a1, register __a2 struct FTPSite *ftps)
{
   struct List *list = list_a1;
   struct FTPEntry *ftpe;

   if(ftpe = (struct FTPEntry *)AllocVec(sizeof(struct FTPEntry), MEMF_CLEAR))
   {
      CopyMem(ftps, (APTR)&ftpe->ftp_Site, sizeof(struct FTPSite));
      ftpe->ftp_Node.ln_Name = ftpe->ftp_Site.Comm1;
      AddTail((struct List *)list, (struct Node *)ftpe);
   }
}
