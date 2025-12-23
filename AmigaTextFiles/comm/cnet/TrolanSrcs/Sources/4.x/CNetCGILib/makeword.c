#include "cnetcgi_lib.h"

char *makeword(char *line, char stop)
{
   int x,
       y;

   char *word = (char *) malloc(sizeof(char) * (strlen(line) + 1));

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

   return word;
}
