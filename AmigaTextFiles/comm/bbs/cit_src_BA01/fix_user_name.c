/**
        Translate the username into a mixed case name
        Rules:
        1) First character of word in a name is uppercase
        2) Second and remaining characters are lowercase
        3) a blank replaces any whitespace characters.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
void      Fix_User_Name(char *name)
  {
  int first_one;
  first_one = TRUE;
  while( *name)
    {
    if( isspace(*name) )
      {
      *name = ' ';  /** replace any whitespace with a blank **/
      first_one = TRUE;
      }
    else if( first_one )
      {
      if( isalpha(*name) )
        {
        *name = toupper(*name);
        };
      first_one = FALSE;
      }
    else if( isalpha(*name) )
      {
      *name = tolower(*name);
      };
    name++;
    };
  }