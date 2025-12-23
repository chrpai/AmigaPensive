/**
   Display a formatted date

   9-june-1998 afp  created initial version
**/
#include "ctdl.h"
#include "math.h"
#include "dos.h"
#include "ctype.h"
#include "ansisys.h"

extern CONFIG    cfg;         /* A buncha variables   */
extern logBuffer logBuf;      /* Pippul buffer    */
extern char      loggedIn;  /* Are we logged in?    */

typedef enum { STANDARD, LONG_DATE, SHORT_DATE } date_type;

static char new_date[20];
char *DisplayDate(char *normal_date)
  {
  /**
    date is input in "normal form", 100JAN01 is 2000 jan 01

  **/
  date_type  the_format;
  int  year;
  char *mon;

  if ( !loggedIn )
    {
    if( cfg.BoolFlags.DFDATE == 0 )
      {
      the_format = STANDARD;
      }
    else
      {
      the_format = ( cfg.BoolFlags.LGDATE == 0 ) ? SHORT_DATE : LONG_DATE;
      };
    }
  else
    {
    if( logBuf.lbflags.DF_DATE == 0 )
      {
      the_format = STANDARD;
      }
    else
      {
      the_format = ( logBuf.lbflags.LG_DATE == 0 ) ? SHORT_DATE : LONG_DATE;
      };
    };
  if( the_format == STANDARD   ) return normal_date;
  year = atoi(normal_date);
  if( year >=0     && year <=   65 ) year += 70;  /* fix funny dates */
  if( the_format == SHORT_DATE )
    {
    year %= 100;
    }
  else
    {
    year += 1900;
    };
    mon = normal_date;
    while( isdigit(*mon) )mon++;  /* skip past year */
    sprintf(new_date,"%02d %s",year,mon);
  return new_date;
  }
