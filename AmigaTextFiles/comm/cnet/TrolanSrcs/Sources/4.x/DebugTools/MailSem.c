/*
** Debugging Tool to Print the status of Mail Semaphores
** by Kelly Cochran
** Last updated: Mon Mar 16 21:21:46 1998
*/

int main(void)
{
   struct MainPort        *myp = NULL;
   struct SignalSemaphore *SEM = NULL;

   int i;

   Forbid();
   myp = (struct MainPort *)FindPort("cnetport");
   Permit();

   if(!myp)
   {
      Printf("Unable to find CONTROL\n");
      return RETURN_FAIL;
   }

   Printf("&myp->MPE          == %lx\n", (LONG)&myp->MPE);
   Printf("&myp->MPE->MailSem == %lx\n", (LONG)&myp->MPE->MailSem);
   Printf("&myp->MPE->MailSem[0]              == %lx\n", (LONG)&myp->MPE->MailSem[2]);
   Printf("&myp->MPE->MailSem[0].ss_NestCount == %x\n", (LONG)myp->MPE->MailSem[2].ss_NestCount);

   if(myp->MPE)
   {
      SEM = myp->MPE->MailSem;

      if(SEM)
      {
         for(i = 0; i < myp->Nums[0]; i++)
         {
            Printf("MailSem[%2ld]: %ld\n", i, SEM[i].ss_NestCount);
         }
      }
   }

   return RETURN_OK;
}
