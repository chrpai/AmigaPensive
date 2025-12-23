#include "cnetcgi_lib.h"

void getword(char *word, char *line, char stop)
{
   int x,
       y;

   for(x = 0; ((line[x]) && (line[x] != stop)); x++)
   {
      word[x] = line[x];
   }

   word[x] = '\0';

   if(line[x])
   {
      ++x;
   }

   y = 0;

   while(line[y++] = line[x++]);
}
