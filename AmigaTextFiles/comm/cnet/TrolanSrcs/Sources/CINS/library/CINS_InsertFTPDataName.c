#include "cins_protos.h"
#include <cins/config.h>

void __saveds __asm CINS_InsertFTPDataName(register __a1 struct List *list_a1, register __a2 struct FTPSite *ftps, register __d1 int pos_d1)
{
   int pos = pos_d1;
   struct List *list = list_a1;
   struct FTPEntry *ftpe,
                   *work = (struct FTPEntry *)CINS_FindOrdinal(list, pos);

   if(ftpe = (struct FTPEntry *)AllocVec(sizeof(struct FTPEntry), MEMF_CLEAR))
   {
      CopyMem(ftps, (APTR)&ftpe->ftp_Site, sizeof(struct FTPSite));
      ftpe->ftp_Node.ln_Name = ftpe->ftp_Site.Comm1;
      Insert((struct List *)list, (struct Node *)ftpe, (struct Node *)work->ftp_Node.ln_Pred);
   }
}
