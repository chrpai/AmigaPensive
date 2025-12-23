/* this utility will make two passes over the current directory and
** the filedir.txt. The first pass will remove missing files from
** the entries, the second will add new ones.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysdep.h"

int parsename(char *, char *);
int  main(int,char **);
int  main(int argc,char **argv)
  {
  FILE *fopen(), *fd, *fo;
  char line[8000], *fgets(), name[20];
  fprintf(stderr,"Citadel CULLDIR %s\n%s", VERSION_NAME, COPYRIGHT );
  if ((fd = fopen("filedir.txt", "r")) == NULL)
    {
    printf("Couldn't find a filedir.txt.");
    return 10;
    }
  else
    {
    if ((fo = fopen("$tmp0000.dat", "w")) == NULL)
      {
      printf("Couldn't create temporary file");
      return 10;
      }
    else
      {
      printf("...processing data\n");
      while (fgets(line, 7998, fd) != NULL)
        {
        if (parsename(line, name))
          {
          if (access(name, 0) == 0)
            {
            fprintf(fo, "%s", line);

            }
          else if (line[0] == ';') fprintf(fo, "%s", line);

          }
        else if (line[0] == ';') fprintf(fo, "%s", line);

        }
      fclose(fo);
      }
    fclose(fd);
    unlink("filedir.txt");
    rename("$tmp0000.dat", "filedir.txt");
    }
  return 0;
  }
parsename(src, target)
char *src, *target;
  {
  char *strchr(), *space;
  if ((space = strchr(src, ' ')) != NULL)
    {
    while (src != space)
    *target++ = *src++;
    *target = 0;
    return 1;

    }
  if (*src != ';')
    {
    strcpy(target, src);
    if ((space = strchr(target, '\n')) != NULL) *space = 0;
    return 1;

    }
  return 0;

  }
