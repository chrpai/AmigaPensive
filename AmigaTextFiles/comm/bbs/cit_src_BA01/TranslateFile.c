#include "ctdl.h"
/*
* TranslateFilename()
*
* This does translations on a filename.  This is used for embedding dates
* or numbers into a filename.
*
* A %m is replaced with the text of the month.
* a %y is replaced with the normalized year digits(i.e., year 2000 is
* 100) of the date
*
* fixed to not allow spaces in the filename.
* fixed to be a bit faster...  no need to search for the end of the name.
* sprintf returns the number of characters in the string output.
*/
void TranslateFilename(
  char *realfn,    /* filename after formatting */
  char *fn)        /* filename template to process */
  {
  int year, day, hours, minutes;
  char *month;
  getCdate(&year, &month, &day, &hours, &minutes);
  do
    {
    *realfn = *fn;
    if (*fn == '%')
      {
      fn++;
      switch (*fn)
        {
        case ' ': *realfn = '\0'; return;  /* eliminate spaces in names */
        case 'm':
        case 'M': realfn += sprintf(realfn, "%s", month); break;
        case 'y':
        case 'Y': realfn += sprintf(realfn, "%d", year);  break;
        default:  realfn += sprintf(realfn, "%c", *fn);   break;
        };
      *realfn = '\0';
      };
    }  while (*fn++);
  }
