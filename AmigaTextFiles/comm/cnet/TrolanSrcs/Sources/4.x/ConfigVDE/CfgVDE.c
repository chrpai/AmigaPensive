/*
** VDE for CNet/Config
** Copyright 1996-8
** by Kelly Cochran
** Last updated: Wed Jan 28 21:55:47 1998
*/

#include "CfgVDE.h"

void  do_config(void);

struct PortData *z;
struct MainPort *myp;
struct Library  *CNetCBase = NULL;

void main( void )
{
   struct args
   {
      ULONG port;
   };

   struct CNetCContext     *context;
   struct RDArgs __aligned *rda;
   struct args myargs;

   memset(&myargs, 0, sizeof(struct args));

   if(rda = ReadArgs("PORT/A", (LONG *)&myargs, NULL))
   {
      if(CNetCBase = OpenLibrary("cnetc.library", 4))
      {
         if(context = CNC_InitContext((char *)myargs.port))
         {
            z   = context->z;
            myp = context->myp;

            CNC_SetDoing("CNet/Config");

            do_config();

            CNC_ShutDown(NULL);
            CNC_DisposeContext();
         }
         CloseLibrary(CNetCBase);
      }
      FreeArgs(rda);
   }
}

void do_config(void)
{
   LONG tmp;
   BPTR fp;

   if(fp = Open("SysText:VDE/CNetConfig", MODE_NEWFILE))
   {
      FWrite(fp, (char *)CfgVDE, sizeof(struct VDEentry), VDE_SIZE(CfgVDE));
      Close(fp);
   }

   tmp = (LONG)z->uz[0]; /* Save old z->uz[0] */

   z->uz[0] = TRUE;     /* Ghost for Expansion entries */

   if(CNC_VisualDataEditor("CNetConfig", &myp->gc, sizeof(struct NewConfig1)))
   {
      CNC_PutText("yay");
   }

   z->uz[0] = tmp;      /* Restore old z->uz[0] */
}
