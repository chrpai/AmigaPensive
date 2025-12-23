/*
** El Cheapo MiamiDx (BETA) Toggle Script
** by Kelly Cochran
** Last updated: Thu Oct 01 20:33:01 1998
*/

#include <proto/miami.h>

struct ExecBase   *SysBase = NULL;
struct DosLibrary *DOSBase = NULL;

const char *version = "\0$VER: MiamiDxToggle 1.2 "__AMIGADATE__;

enum
{
   ARG_IFS = 0,
   NUM_ARGS
};

#define TEMPLATE "INTERFACES/A/M"

void __saveds main(void)
{
   struct Library *MiamiBase;

   struct RDArgs *rda;

   LONG args [ NUM_ARGS ];

   SysBase = *(struct ExecBase **)4L;

   if(DOSBase = (struct DosLibrary *)OpenLibrary(DOSNAME, 37L))
   {
      if(rda = ReadArgs(TEMPLATE, args, NULL))
      {
         if(!(MiamiBase = OpenLibrary("miami.library", 10L)))
         {
            SystemTags("Run <>NIL: Miami:MiamiDx.beta", TAG_END);

            Delay(30);     /* should be long enough */

            MiamiBase = OpenLibrary("miami.library", 10L);
         }

         if(MiamiBase)
         {
            char **temp = (char **)args[0];

            while(temp && *temp)
            {
               MiamiOnOffline(*temp, !MiamiIsOnline(*temp));
            }

            CloseLibrary(MiamiBase);
         }

         FreeArgs(rda);
      }

      CloseLibrary((struct Library *)DOSBase);
   }
}

#if 0

/*  ARexx to do (almost) the same thing */
options results

if ~show('p','MIAMI.1') then do;
  address command 'Run <>NIL: Miami:Miami'
  exit
end
else do;
   address MIAMI.1 'ISONLINE'
   if RC = 1 then do;
      address MIAMI.1 'OFFLINE'
   end
   else address MIAMI.1 'ONLINE'
end

#endif
