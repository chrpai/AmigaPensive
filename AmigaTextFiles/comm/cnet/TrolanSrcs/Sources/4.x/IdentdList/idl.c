/*
** CNet Identd Lister
** by Kelly Cochran
** Last updated: Wed Sep 17 00:39:29 1997
*/

void main(int argc, char **argv)
{
   struct IdentdData *iddptr;
   struct MainPort   *myp;

   Forbid();
   if(!(myp = (struct MainPort *)FindPort("cnetport")))
   {
      Permit();
      Printf("Could not find CONTROL process\n");
      exit(0);
   }
   Permit();

   ObtainSemaphore(&myp->MPE->sem[1]);

   if(myp->MPE->idd)
   {
      iddptr = myp->MPE->idd;

      while(iddptr)
      {
         Printf("Socket number: %ld, CNet Owner Port: %ld\n", iddptr->socket, iddptr->port);
         iddptr = iddptr->next;
      }
   }
   else
   {
      Printf("No identd entries\n");
   }

   ReleaseSemaphore(&myp->MPE->sem[1]);

   exit(0);
}
