/*
** DirMake - Utility to make subdirectories for CNet/4
** by Kelly Cochran - 1997
** Rights are granted to use this source for personal use only
**
** ZenMetal Software has the additional right to include this
**  source as code examples for CNet/4
*/

void main(void)
{
   struct Library  *CNetBase;
   struct MainPort *myp;

   char buff [ 256 ] = "";
   int i;

   Forbid();
   myp = (struct MainPort *)FindPort("cnetport");
   Permit();

   if(!(CNetBase = OpenLibrary("cnet.library", 4)))
      exit(1);

   if(!myp)
      exit(2);

   for(i = 0; i < myp->ns; i++)  /* thru all the subs */
   {
      Printf("%s\n", myp->Subboard[i].Title);
      sprintf(buff, "%s", myp->Subboard[i].ZeroPath); /* Part/Base/Net */
      if(!FileExists(buff))
      {
         if(buff[strlen(buff) - 1] == '/')   /* trim off trailing '/' */
            buff[strlen(buff) - 1] = 0;
         BuildDir(buff);
         Printf("\tCreated %s\n", buff);
      }
      if(myp->Subboard[i].Marker < 2)
      {
         sprintf(buff, "%sdata", myp->Subboard[i].DataPath); /* data dir */
         if(!FileExists(buff))
         {
            BuildDir(buff);
            Printf("\tCreated %s\n", buff);
         }
      }
   }
   CloseLibrary(CNetBase);
}
