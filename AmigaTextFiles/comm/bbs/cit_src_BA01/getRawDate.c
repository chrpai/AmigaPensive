#include <time.h>
/**
   getRawDate

   Input:  System time

   Output:  numeric year    (4 digits)
            numeric month   ( 1-12)
            numeric day     ( 1-31)
            numeric hour    ( 0-23)
            numeric minutes ( 0-59)
            numeric milli   ( 0-999)
**/
void getRawDate(int *year, int *month, int *day, int *hours, int *minutes,
int *seconds, int *milli)
  {
  long t;
  struct tm *v;
  time(&t);
  v        = localtime(&t);
  *year    = v->tm_year + 1900;
  *month   = v->tm_mon + 1;
  *day     = v->tm_mday;
  *hours   = v->tm_hour;
  *minutes = v->tm_min;
  *seconds = v->tm_sec;
  *milli   = 0;

  }

