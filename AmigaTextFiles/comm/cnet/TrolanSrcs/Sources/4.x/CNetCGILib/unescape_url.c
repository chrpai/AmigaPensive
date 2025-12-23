#include "cnetcgi_lib.h"

void unescape_url(char *url)
{
   register int x,y;

   for(x=0,y=0;url[y];++x,++y)
   {
      if((url[x] = url[y]) == '%')
      {
         url[x] = x2c(&url[y+1]);
         y+=2;
      }
   }
   url[x] = '\0';
} /* unescape_url() */

