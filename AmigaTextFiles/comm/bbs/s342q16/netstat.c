/**
  Network Statistics Version 1.02:

  Input:
      Datafile created by Citadel
      cc       1    1     2    2 2 3      3 34       4 5        5
      1   5 7  0    5     0    6 8 0      7 90       8 0        9
      MMMYY xxxxxxxxxxxxxxxxxxxx nnnnnnnnnn nnnnnnnnnn nnnnnnnnnn
            Name of Net Partner  chars in   chars out    time

  Name of datafile is 1st argument.
  Optional arguments:
   flag
     -S    to report by system.
     -M    to report totals on a monthly basis.

  Output:
      Report

**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <math.h>

struct bbs
  {
  struct bbs *next;     /* next bbs system */
  char name[21];        /* name of bbs */
  long net_count[12];   /* number of network sessions */
  long chars_in[12];    /* monthly totals of characters input */
  long chars_out[12];   /* monthly totals of characters output */
  long time_used[12];   /* monthly totals of time used in networking */
  long min_cps[12];         /* minimum cps that occured */
  long max_cps[12];         /* maximum cps that occured */
  };

struct bbs *systems = NULL; /* list of systems */
int sflag = FALSE;      /* no individual per system statistics */
int mflag = FALSE;      /* no monthy statistics */
int lineno= 0;          /* current line number in the file */
void Collect_Statistics(FILE *ip);
void Report_Statistics(void);
int main(int argc, char *argv[]);
struct bbs *Find_System(char *name);
void Report_BBS(struct bbs *ptr);
int Date_to_Index(char *date);


int main(int argc, char *argv[])
  {
  FILE *ip;
  char *name=NULL;
  printf("   Citadel Networking Statistics V1.02\n");
  while( --argc )
    {
    if( argv[argc][0] == '-')
      {
      if( argv[argc][1] == 'S') sflag = TRUE;
      if( argv[argc][1] == 's') sflag = TRUE;
      if( argv[argc][1] == 'M') mflag = TRUE;
      if( argv[argc][1] == 'm') mflag = TRUE;
      }
     else name = argv[argc];
     };
  if( name == NULL)
    {
    fprintf(stderr,"Error, need a file to process\n");
    return(100);
    };
  if( (ip=fopen(name,"r")) == NULL )
    {
    fprintf(stderr," Error opening %s for input\n",argv[argc]);
    return(100);
    };
  Collect_Statistics(ip);
  fclose(ip);
  Report_Statistics();
  return 0;
  }

void Collect_Statistics(FILE *ip)
  {
  struct bbs *ptr;
  char line[80];
  int did;
  while( fgets(line, sizeof(line), ip) )
    {
    long  nci, nco, ntm;
    lineno++;
    line[ 3] = '\0';
    line[27] = '\0';
    line[38] = '\0';
    line[49] = '\0';
    line[60] = '\0';
    did  = Date_to_Index(&line[ 0]);
    ptr = Find_System(&line[ 7]);
    ptr->net_count[did]++;
    ptr->chars_in[did]  += (nci = atol(&line[28]));
    ptr->chars_out[did] += (nco = atol(&line[39]));
    ptr->time_used[did] += (ntm = atol(&line[50]));
    nci += nco;
    if( ntm > 0 )
      {
      nco = nci / ntm;
      if( nco < ptr->min_cps[did]) ptr->min_cps[did] = nco;
      if( nco > ptr->max_cps[did]) ptr->max_cps[did] = nco;
      };
    };
  }

char *month[12] =
  {
  "jan", "feb", "mar", "apr", "may", "jun",
  "jul", "aug", "sep", "oct", "nov", "dec"
  };


int Date_to_Index(char *date)
  {
  int i;
  if( ! mflag )return 0;  /* if not doing monthly stats, skip this */
  for( i=0; i<12; i++)
    {
    if( stricmp(month[i],date)== 0 )return i;
    };
  fprintf(stderr,"Error: INVALID DATE at line %d\n",lineno);
  return 0;
  }

struct bbs *Find_System(char *name)
  {
  struct bbs *ptr=systems;
  int i;
  if( sflag )
    {
    while ( ptr != NULL )
      {
      if( strcmp(ptr->name,name) == 0)return ptr;
      ptr = ptr->next;
      };
    if( ptr == NULL )
      {
      ptr = calloc(1, sizeof(struct bbs));
      strcpy(ptr->name,name);
      for(i=0; i<12; i++)
        {
        ptr->min_cps[i] = 99999;
        ptr->max_cps[i] = -1;
        };
      ptr->next    = systems;
      systems      = ptr;
      };
    }
  else
    {
    if( systems == NULL )
      {
      ptr = systems = calloc(1, sizeof(struct bbs));
      strcpy(ptr->name," *** All Systems ***");
      for(i=0; i<12; i++)
        {
        ptr->min_cps[i] = 99999;
        ptr->max_cps[i] = -1;
        };
      };
    };
  return ptr;
  }

void Report_Statistics(void)
  {
  long hr, sec, mins, avr_cps;
  struct bbs *ptr=systems;
  printf("   ----- Name -----     Session     Input     Output     Time          CPS\n");
  printf("                         count                         HH:MM:SS   Min  Avr  Max\n");
  Report_BBS(ptr);
  if( sflag )
    {
    hr  = ptr->time_used[0];
    sec = hr % 60;
    hr /= 60;
    mins = hr %60;
    hr /= 60;
    if( ptr->time_used[0] != 0)
      {
      avr_cps = (ptr->chars_in[0] + ptr->chars_out[0] ) / ptr->time_used[0];
      }
    else avr_cps = 0;
    printf("%23s-------  ---------  ---------   --------\n"," ");
    printf(" %20s  %7ld %10ld %10ld  %3ld:%02ld:%02ld       %4ld\n",
    " ", ptr->net_count[0], ptr->chars_in[0], ptr->chars_out[0], hr, mins, sec
    ,  avr_cps);
    };
  }

void Report_BBS(struct bbs *ptr)
  {
  int idx;
  long avr_cps;
  long hr, mins, sec;
  if( ptr->next != NULL) Report_BBS(ptr->next);
  for( idx=0; idx < 12 && mflag; idx++)
    {
    if( ptr->time_used[idx] == 0)continue;
    hr  = ptr->time_used[idx];
    sec = hr % 60;
    hr /= 60;
    mins = hr %60;
    hr /= 60;
    avr_cps = (ptr->chars_in[idx] + ptr->chars_out[idx] ) / ptr->time_used[idx];
    printf(" %20s  %7ld %10ld %10ld  %3ld:%02ld:%02ld  %4ld %4ld %4ld\n",
      month[idx], ptr->net_count[idx], ptr->chars_in[idx], ptr->chars_out[idx], hr, mins, sec,
      ptr->min_cps[idx], avr_cps, ptr->max_cps[idx]);
    if( idx > 0)
      {
      ptr->net_count[0] += ptr->net_count[idx];
      ptr->chars_in[0]  += ptr->chars_in[idx];
      ptr->chars_out[0] += ptr->chars_out[idx];
      ptr->time_used[0] += ptr->time_used[idx];
      if( ptr->min_cps[0] > ptr->min_cps[idx]) ptr->min_cps[0] = ptr->min_cps[idx];
      if( ptr->max_cps[0] < ptr->max_cps[idx]) ptr->max_cps[0] = ptr->max_cps[idx];
      };
    };

  hr  = ptr->time_used[0];
  sec = hr % 60;
  hr /= 60;
  mins = hr %60;
  hr /= 60;
  if( ptr->time_used[0] != 0)
    {
    avr_cps = (ptr->chars_in[0] + ptr->chars_out[0] ) / ptr->time_used[0];
    }
  else avr_cps = 0;
  if( mflag  )
    printf("%23s-------  ---------  ---------   --------\n"," ");
  printf(" %20s  %7ld %10ld %10ld  %3ld:%02ld:%02ld  %4ld %4ld %4ld\n",
  ptr->name, ptr->net_count[0], ptr->chars_in[0], ptr->chars_out[0], hr, mins, sec
  , ptr->min_cps[0], avr_cps, ptr->max_cps[0]);
  if( mflag )printf("\n");
  if( ptr->next != 0 && sflag )
      {
      ptr->chars_in[0]  += ptr->next->chars_in[0];
      ptr->chars_out[0] += ptr->next->chars_out[0];
      ptr->time_used[0] += ptr->next->time_used[0];
      ptr->net_count[0] += ptr->next->net_count[0];
      };

  }
