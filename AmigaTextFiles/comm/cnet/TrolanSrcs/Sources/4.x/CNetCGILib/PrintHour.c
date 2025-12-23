#include "cnetcgi_lib.h"

void PrintHour(struct IsDate *date, char *dout)
{
   char ap   = 'a';
   int  hour = date->Hour;

   if(hour > 11)
   {
      hour -= 12;
      ap   = 'p';
   }
   if(!hour)
   {
      hour = 12;
   }
   sprintf(dout, "%2d:%02d%c", hour, date->Minute, ap);
} /* PrintHour */
