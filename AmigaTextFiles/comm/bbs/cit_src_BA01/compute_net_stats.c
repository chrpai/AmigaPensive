/**
  functions for computing the network data traffic
**/
#include "ctdl.h"
#include "string.h"
#include "stdarg.h"
#include "time.h"
#define EXTERN extern
#include "global_data.h"

extern char  *monthTab[13];

static char lognet[80];
static char date2[6], date1[6];
static void Write_Total_Data(void);
long total_time;               /* total time for all net sessions */
long total_char_in;            /* totals for all time */
long total_char_out;           /* totals for all time */
extern long char_in;                  /* totals for current time */
extern long char_out;                 /* totals for current time */
extern CONFIG      cfg;               /* Configuration variables      */
extern long start_time;               /* total time for net session */

void makeAuditName(char *logfn, char *name);
void Read_Total_Data(void);
void getCdate(int *year, char **month, int *day, int *hours, int *minutes);

void makeAuditName(char *logfn, char *name)
  {
  char *ADir;
  int i;
  ADir = cfg.auditArea.saDirname;
  i = strlen(ADir);
  if( ADir[i-1] == '/' || ADir[i-1] == ':' )
    {
    sPrintf(logfn, "%s%s", ADir,  name);
    }
  else    sPrintf(logfn, "%s/%s", ADir,  name);
  }

void Read_Total_Data()
  {
  char *mon;
  FILE *ip;
  int lyr, ldum;
  if( cfg.Audit == 0 )return;
  makeAuditName(lognet, "Network_stats.sys");
  if( (ip = fopen(lognet, "r")) != NULL )
    {
    fread(&date1,         6, 1, ip);
    fread(&total_time,    4, 1, ip);
    fread(&total_char_in, 4, 1, ip);
    fread(&total_char_out,4, 1, ip);
    fclose(ip);
    getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
    sprintf(date2,"%3s%2d",mon,lyr);      /* monthYear */
    if( strcmp(date1,date2) != 0 )
      {
      /* reset, new month  */
      getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
      sprintf(date1,"%3s%2d",mon,lyr);      /* monthYear */
      total_time    = 0;
      total_char_in = 0;
      total_char_out= 0;
      Write_Total_Data();
      }
    }
  else   /* no file, create it */
    {
    getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
    sprintf(date1,"%3s%2d",mon,lyr);      /* monthYear */
    total_time    = 0;
    total_char_in  = 0;   /* new file */
    total_char_out = 0;
    Write_Total_Data();
    }
  }

static void Write_Total_Data()
  {
  FILE *ip;
  if( cfg.Audit == 0 )return;
  makeAuditName(lognet, "Network_stats.sys");
  if( (ip = fopen(lognet, "w")) != NULL )
    {
    fwrite(date1,          6, 1, ip);
    fwrite(&total_time,    4, 1, ip);
    fwrite(&total_char_in, 4, 1, ip);
    fwrite(&total_char_out,4, 1, ip);
    fclose(ip);
    }
  }

void Compute_Data(char *name)
  {
  int lyr,ldum;
  char *mon;
  FILE *ip;
  long cps;           /* computed data rate */
  long work_time;
  work_time = Set_Timer(start_time);
  if( work_time == 0 ) work_time = 1;
  cps = ( char_in + char_out ) / work_time;
  printf(" Characters Input:%ld Characters Output:%ld  %ld cps\n", char_in, char_out, cps);
  if( cfg.Audit  )
    {
    makeAuditName(lognet,"Network_Istats.sys");
    if( (ip = fopen(lognet, "a")) != NULL)
      {
      getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
      sprintf(date2,"%3s%2d",mon,lyr);      /* monthYear */
      fprintf(ip,"%s %20s %10ld %10ld %10ld\n",date2,name, char_in, char_out, work_time);
      fclose(ip);
      };
    };
  Read_Total_Data();
  total_time     += work_time;
  total_char_in  += char_in;
  total_char_out += char_out;
  Write_Total_Data();
  }
/*
 * getCdate()
 *
 * This retrieves system date and returns in the parameters.
 */
void
getCdate (int *year, char **month, int *day, int *hours, int *minutes)
{
  int       mon,
            seconds,
            milli;

  getRawDate (year, &mon, day, hours, minutes, &seconds, &milli);
  *year -= 1900;
  *month = monthTab[mon];

}

unsigned long Set_Timer(unsigned long oldtime ) /* return the delta from oldtime in microsecs*/
  {
  long t;
  (void)time(&t);
  return ( t - oldtime );
  }

