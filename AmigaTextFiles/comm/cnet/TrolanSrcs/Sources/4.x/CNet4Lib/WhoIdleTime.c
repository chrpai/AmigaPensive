#include "cnet4lib.h"

/*
** Pass PortData struct, output buffer, and minimum time to count idle
** returns TRUE if z->TimeIdle > mintime, and FALSE otherwise
*/

BOOL WhoIdleTime(struct PortData *z, char *output, int mintime)
{
   if(z)
   {
      ULONG idle = z->TimeIdle;

      if(idle > mintime)
      {
         if(idle > 300)
         {
            sprintf(output, z->bm[1124], (idle/60)/10, (idle/60)%10, z->bm[1126]);
         }
         else
         {
            sprintf(output, z->bm[1124], idle/10, idle%10, z->bm[1125]);
         }

         return TRUE;
      }
   }

   return FALSE;
}
