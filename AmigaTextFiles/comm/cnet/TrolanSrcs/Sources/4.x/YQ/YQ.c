/*
** CLI Yank Queue Scanner for CNet/4
** by Kelly Cochran
** Last updated: Wed Sep 17 01:02:29 1997
*/

void main(void)
{
   int i = 0;

   char status [ 128 ] = "";
   char *type;

   struct MainPort *myp;
   struct YankType *ytype;
   struct Library  *CNet4Base;

   if(CNet4Base = OpenLibrary("cnet4.library", 4))
   {
      Forbid();
      if(myp = (struct MainPort *)FindPort("cnetport"))
      {
         Permit();
         ObtainSemaphore(&myp->ysem);
         ytype = myp->yfirst;

         if(ytype)
         {
            Printf("## Job ID   Type Time     Owner                Status\n");
            Printf("== ======== ==== ======== ==================== =========================\n");
         }

         while(ytype)
         {
            switch(ytype->parm & 0x07)
            {
               case 1:
               case 2:
                  type = myp->FirstTT->bm[2114];
                  break;
               default:
                  type = myp->FirstTT->bm[2213];
                  break;
            }
/*
            if(ytype->parm & 0x08)
               delete _marked3 before marking
            if(ytype->parm & 0x10)
               also yank email
            if(ytype->parm & 0x20)
               yank current item only
*/
            if(ytype->status)
               sprintf(status, myp->FirstTT->bm[2112], ytype->nmess, (ytype->ymess + 500)/1000);
            else
               strcpy(status, myp->FirstTT->bm[2111]);

            printf("%2d %08x %s %2d:%02d:%02d %-20s %s\n", i+1, ytype->yid, type,
                   ytype->Submitted.Hour, ytype->Submitted.Minute, ytype->Submitted.Second,
                   myp->Key[ytype->id - 1].Handle, status);
            ytype = ytype->next;
            i++;
         }
         ReleaseSemaphore(&myp->ysem);

         if(!i)
            Printf("The Yank Queue is empty.\n");
      }
      else
      {
         Permit();
         Printf("Could not find CONTROL process.\n");
      }
      CloseLibrary(CNet4Base);
   }
}
