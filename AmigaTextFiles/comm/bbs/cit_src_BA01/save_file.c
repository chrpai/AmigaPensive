#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv);

int main (int argc, char **argv)
  {
  char name[80];
  int i = 0;
  /**
    arg 1:  filename to save from current directory
    arg 2:  directory to move the file to

    1) check arguments
    2) find a valid filename in the directory
    3) copy the file
  **/
  if( argc != 3 )
    {
    printf("Usage:  %s <file> <dir>\n",argv[0]);
    return (100);
    };
  if( access( argv[1], R_OK ) != 0 )
    {
    perror("save_file<file>");
    (void)poserr("save_file <file>");
    return (100);
    };
  if( access( argv[2], R_OK ) != 0 )
    {
    perror("save_file<dir>");
    (void)poserr("save_file<dir>");
    return (100);
    };
  sprintf(name,"%s/%s.%06d", argv[2], argv[1], i++);
  while (access(name, F_OK) != -1)   sprintf(name,"%s/%s.%06d", argv[2], argv[1], i++);
  if( rename( argv[1], name) )
    {
    printf("Failed to rename %s to %s\n",argv[1], name);
    perror("save_file<rename>");
    (void)poserr("save_file<rename>");
    return (100);
    };
  return 0;
  }

