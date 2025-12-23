/**
    Copyrighted 1998 Custom Services, All rights reserved
    This code may be used by anybody except Bill Gates,
    his representatives, or Microsoft Inc.

    routine to fix a Citadel date string into
    a "normalized" format

    Input:  date string in the format  y[yy]ymmmdd

    Output:  Update the date in place.

    Note:  the date passed will be overwritted with the
    corrected date.

    Note:  logfile is your error logs file pointer.  On an erro
    nothing is done which might screw things up royally...

**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dos.h>
#include <ctype.h>

#include "ctdl.h"

extern FILE      *netLog;


  void NormalizeDate(char *udate )
     {
     int bad;
     int year;
     char *mon_day;
     char newdate[10];

     if( udate == NULL )
       {
       splitF(netLog, "Error: Null date.\n");
       return;
       };
     if( *udate == '*') return;  /* anonymous message, no date */
     if( strlen(udate) > 9 )
       {
       splitF(netLog, "Error: date(%s) is invalid!\n",udate);
       /* might want to set udate to today's date */
       return;
       };
     strcpy(newdate,udate);
     year = atoi(udate);
     bad = TRUE;
     if( year >=0     && year <=   65 ) year += 70;  /* fix funny dates */
     if( year >=   70 && year <=  135 ) bad = FALSE;
     if( year >= 1970 && year <= 2205 ) bad = FALSE;
     if( bad )
       {
       splitF(netLog, "Error: date(%s) is out of range!\n",udate);
       /* might want to set udate to today's date */
       return;
       };
     if( year >= 1970 ) year -= 1900;
     mon_day = &udate[2];
     while( isdigit(*mon_day) )mon_day++;
     sprintf(newdate,"%d%s",year, mon_day);
     strcpy(udate,newdate);
     return;
}