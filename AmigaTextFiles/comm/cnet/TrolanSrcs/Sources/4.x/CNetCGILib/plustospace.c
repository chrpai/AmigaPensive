#include "cnetcgi_lib.h"

void plustospace(char *str)
{
   register int x;
   for(x = 0; str[ x ]; x++)
      if(str[ x ] == '+')
         str[ x ] =  ' ';
} /* plustospace */
