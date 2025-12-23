/*
** El Cheapo Miami Toggle Script
** by Kelly Cochran
** Last updated: Tue Apr 28 23:11:32 1998
*/

#include <proto/miami.h>

struct ExecBase   *SysBase = NULL;
struct DosLibrary *DOSBase = NULL;

const char *version = "\0$VER: MiamiToggle 1.2 "__AMIGADATE__;

void __saveds main(void)
{
   struct Library *MiamiBase;

   SysBase = *(struct ExecBase **)4L;

   if(DOSBase = (struct DosLibrary *)OpenLibrary(DOSNAME, 37L))
   {
      if(MiamiBase = OpenLibrary("miami.library", 10L))
      {
         MiamiOnOffline("mi0", !MiamiIsOnline("mi0"));

         CloseLibrary(MiamiBase);
      }
      else
      {
         SystemTags("Run <>NIL: Miami:Miami", TAG_END);
      }

      CloseLibrary((struct Library *)DOSBase);
   }
}

#if 0

/*  ARexx to do the same thing */
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
