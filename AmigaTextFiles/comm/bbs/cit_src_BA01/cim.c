#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <dos/dos.h>
#include <clib/dos_protos.h>
#include <pragmas/dos_pragmas.h>
#include "sysdep.h"
extern struct DosLibrary *DOSBase;
/**
  Citadel Internet Mailer Version 1.00

  Input:  parameter filename
  Output:
 This tool is designed to work with AirMail Version 3  It will queue
 a mail into the directory you specify(it should be the out box for AirMail).

 Since AirMail has no control over the file for the new mail, this tool
 will create the new mail with a header that looks like this:

[AIRMAIL3]                      --> Mail keyword.
To: toysoft@connect.ab.ca,      --> Must come before the TO: address/s.
From: toysoft@connect.ab.ca     --> Automatically displayed.                            --> Must come before the DATE.
Date: Nov 24 1995 11:11:09 PM   --> Automatically displayed.
Subject: Air Mail is great!     --> Automatically displayed or entered.
[FILES]                         --> Must come before an attached FILE.
X-Mailer: Citadel Internet Mailer V 6J20
 <blank line>
1. The maximum number of characters per line in the header in 60 characters.
2. You can have as many lines in the TO: and [FILES] block.
3. A ', ' comma and space separates each email address or file.
4. A ', ' comma and space terminates each line in the [TO] and [FILES] block
5. There must be a blank line between the last line in the header block
   and your message.

**/
int month_number(char *name);
int main(int argc, char **argv);

#define IN_PARAMS  (7)

char  *param[IN_PARAMS];


int month_number(char *name)
  {
  switch (name[0])
    {
    case 'A':  /* april or august */
      return ( name[1] == 'U' ? 8 : 4 );
    case 'D':  /* december */
      return 12;
    case 'F':  /* febuary */
      return 2;
    case 'J':  /* january, june or july */
      return ( name[1] == 'A' ? 1 : ( name[2] == 'L' ? 7 : 6 ) );
    case 'M':  /* may or march */
      return ( name[2] == 'R' ? 3 : 5 );
    case 'N':  /* november */
      return 11;
    case 'O':  /* october */
      return 10;
    };
  return 9;  /* september */
  }

int main(int argc, char **argv)
  {
  time_t  timeval;
  struct tm *tp;
  long t;
  long count;
  int found;  /* flags */
  int flag;
  char buffer[128];
  char line[120];
  char times[26];
  char filename[20];
  FILE *mail_file;
  FILE *mail_out;
  FILE *p_file;
  char *ptr;
  int i;
  printf("Citadel Internet Mailer %s\n", VERSION_NAME);
  if( argc != 2 )
    {
    printf("Usage: CIM <Parameter File> \n");
    return 100;
    };
  /**
    Read the parameter file and validate that we have access to
    the mail packet file plus it is not zero length.
  **/
  if( ( p_file  = fopen(argv[1],"r") ) == NULL )
    {
    fprintf(stderr,"Error: cannot open parameter file(%s)\n",argv[1]);
    exit(100);
    };
    /**
      Read parameters
    **/
    for(i=0; i<IN_PARAMS; i++)
      {
      if( fgets(line, sizeof(line), p_file) == NULL )
        {
        fprintf(stderr,"Error: EOF while reading parameter file line %d\n",i);
        fclose(p_file);
        exit(100);
        };
      if( ( ptr = strchr(line,'\n') ) != NULL ) *ptr = '\0';
      param[i] = strdup(line);
      };
  /**
    Check to see that we have read access to the Mail file before we
    attempt to create the mail message.
  **/
  if( access(param[5],R_OK) == -1 )
    {
    printf("Error: Unable to open mail file(%s), aborting\n", param[5]);
    exit(100);
    };
  /**
    Open the queued mail file, first we must figure out the file
    name.  The filename is of the form YYYYMMDD<count>.AM, count starts
    at 1 and increments until a filename is not found, then we create that
    file.
  **/
  time(&timeval);
  tp = localtime(&timeval);
  sprintf(times,"%s",asctime(tp) );
  for(t=0; t<4; t++) filename[0+t] = times[20+t]; /* copy year */
  filename[4] = '.';
  sprintf(&filename[5],"%02.2d",month_number(&times[4])); /* month */
  filename[7] = '.';
  for(t=0; t<2; t++) filename[8+t] = times[ 8+t]; /* day of the month */
  filename[10] = '\0';
  /*
   count starts at 1, we look for an empty slot number and add the file
   there.
  */
  t = strlen(param[6])-1;
  flag = FALSE;
  if( param[6][t] == '/' || param[6][t] == ':')flag=TRUE;
  count = 0;
  found = TRUE;
  while ( found )
    {
    count++;
    if( flag )
      {
      sprintf( buffer, "%s%s.%03.3d.AM", param[6], filename, count);
      }
    else sprintf( buffer, "%s/%s.%03.3d.AM", param[6], filename, count);
    if( access(buffer,F_OK) == -1 ) break;
    };
  /*
   found an empty slot, create the file
  */
  if( (mail_out = fopen(buffer,"w")) == NULL )
    {
    printf("Error: Cannot create mail file(%s)\n",buffer);
    exit (100);
    };
  fprintf( mail_out, "[AIRMAIL3]\n");
  fprintf( mail_out, "To: %s,\n",      param[0]);
  fprintf( mail_out, "From: %s\n",     param[1]);
  fprintf( mail_out, "Reply-To: %s\n", param[2]);
  fprintf( mail_out, "Date: %s\n",      &times[4]);
  fprintf( mail_out, "Subject: %s\n",  param[3]);
  fprintf( mail_out, "Organization: %s\n",param[4]);
  fprintf( mail_out, "[FILES]\n");
  fprintf( mail_out, "X-Mailer: Citadel Internet Mailer (6J20)\n\n");
  /* copy the mail file */
  if( (mail_file = fopen(param[5],"r")) == NULL )
    {
    fclose(mail_out);
    printf("Error: cannot open mail to send file(%s)\n",argv[1]);
    exit(100);
    };
  while( fgets(line,sizeof(line), mail_file) ) fputs(line, mail_out);
  fprintf(mail_out,"\n");
  fclose(mail_out);
  fclose(mail_file);
  SetComment( buffer, "QUEUED");
  exit(0);
  }
