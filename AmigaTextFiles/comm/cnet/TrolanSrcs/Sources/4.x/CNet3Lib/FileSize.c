#include "cnet3lib.h"

long FileSize(char *filename)
{
   BPTR fp;

   if(fp = Open(filename, MODE_OLDFILE))
   {
      if(ExamineFH(fp, z->fib))
      {
         Close(fp);
         return (z->fib->fib_Size);
      }
      Close(fp);
   }
   return -1;
}
