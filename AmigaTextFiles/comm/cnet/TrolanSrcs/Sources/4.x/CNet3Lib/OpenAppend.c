#include "cnet3lib.h"

BPTR OpenAppend(char *file)
{
   BPTR fp;

   if(fp = Open(file, MODE_OLDFILE))
   {
      Seek(fp, 0, OFFSET_END);
      return fp;
   }
   else if(fp = Open(file, MODE_NEWFILE))
   {
      return fp;
   }
}
