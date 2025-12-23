int main(void)
{
   struct Library *CNetMailBase;
   struct MainPort *myp;

   printf("Task name?: %s\n", FindTask(NULL)->tc_Node.ln_Name);

   if(myp = (struct MainPort *)FindPort("cnetport"))
   {
      printf("myp->MPE->MailSem: 0x%lx\n", myp->MPE->MailSem);

      if(CNetMailBase = OpenLibrary("cnetmail.library", 4))
      {
         printf("GetMailSems()    : 0x%lx\n", GetMailSems());
         printf("GetMPSems()      : 0x%lx\n", GetMPSems());
         CloseLibrary(CNetMailBase);
      }
   }
}
