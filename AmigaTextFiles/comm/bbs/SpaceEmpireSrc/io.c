#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos/dos.h>
#include <math.h>
#include <time.h>
#include <clib/dos_protos.h>
#include <pragmas/dos_pragmas.h>
#include <ctype.h>
#include "io.h"
extern long DOSBase;

void Prompt(string)
char *string;
  {
  printf("%s:",string);

  }

int Get_Number(string,low,high)
int low;
int high;
char *string;
  {
  register short idx;
  register int number;
  char ibuf[10];
  number = -1;
  while ( number <= -1 )
    {
    Prompt(string);
    for(idx = 0; idx < 10; idx++)ibuf[idx] = 0;
    Terminal(ibuf,10);
    number = atoi(ibuf);
    if( number < low || number > high )
      {
      printf("you entered %d, that is not legal\n",number);
      number = -1;     /* try again */

      };

    };
  return(number);

  }

void Terminal(string,size)
char *string;
int  size;
  {
  char  c;
  short i;
  for( i = 0; i < size; ) string[i++] = '\0';
  for (i = 0; i < size && ( c = getchar() ) != '\n' && c != EOF;i++)
    {
    string[i] = c;
    };
  while ( c != '\n' && c != EOF )c = getchar();
  }

char YesNo(string)
char *string;
  {
  register char c;
  char cs[2];
  c = '\0';
  while (c != 'y' && c != 'n' && c != EOF )
    {
    if ( c !='\0')printf("\nPlease answer with y(es) or n(o)\n");
    printf("%s?:",string);
    Terminal(cs,1);
    c = tolower(cs[0]);
    if ( c == '\n') c = 'y';
    if ( c == '\0') c = 'y';

    };
  if( c == EOF ) c = 'n';
  return(c);

  }

void System_Error(name,string)
char *string, *name;
  {
  printf("\n*****************************************\n");
  printf("\n%s:%s\n",name,string);
  printf("\n*****************************************\n");
  (void)fcloseall();
  exit(100);

  }
void Get_Record(fp, pos, size, buffer)
long pos;      /* record position */
long size;     /* record size */
BPTR fp;     /* file pointer*/
char *buffer; /* buffer for record */
  {
  register int lenth;
  Seek(fp,pos*size,OFFSET_BEGINNING);   /* position to record */
  lenth = Read(fp,buffer,size);   /* read the record    */
  if(lenth != size)
    {
    printf(" length = %d, size = %d, pos = %d, fp=%u, buffer=%u\n"
    ,lenth,      size,      pos,      fp,    buffer);
    Close(fp);
    System_Error("Get_Record","Reading");

    };

  }
void Put_Record(fp, pos, size, buffer)
long pos;      /* record position */
long size;     /* record size */
BPTR fp;     /* file pointer*/
char *buffer; /* buffer for record */
  {
  register int lenth;
  Seek(fp,pos*size,OFFSET_BEGINNING);   /* position to record */
  lenth = Write(fp,buffer,size);   /* Write the record    */
  if(lenth != size)
    {
    printf(" length = %d, size = %d, pos = %d, fp=%u, buffer=%u\n"
    ,lenth,      size,      pos,      fp,    buffer);
    Close(fp);
    System_Error("Put_Record","Reading");

    };

  }
void strip_crlf(string)
char *string;
  {
  int x,y;
  x =  strlen(string);
  for(y =  0; y<=  x; y++)
    {
    if((string[y] ==  '\r')||(string[y] ==  '\n')) string[y] =  0;

    }

  }
void strtoup(string)
char string[];
  {
  int a,b,c,d;
  b =  strlen(string);
  for(a =  0;a!=  b;++a)
    {
    c =  string[a];
    if(islower(c))
      {
      d = c - 'a' + 'A';
      string[a] =  d;

      }

    }

  }
void  pause()
  {
  printf("Hit return to continue...");
  while( getchar() != '\n');
  }

void showfile(mstring)
char mstring[];
  {
  FILE *fp;
  short i;
  char c, text[80];
  if( (fp =  fopen(mstring,"r")) ==  NULL )
    {
    printf("\nThe file %s is missing, please notify your Sysop\n",mstring);
    }
  else
    {
    i =    0;
    while( fgets(text,79,fp) !=   NULL )
      {
      text[79] =  '\0';
      strip_crlf(text);    /* remove trailing crlf stuff */
      printf("%s\n",text);
      if( i++ > 22)
        {
        printf("Q to quit, Hit return to continue...");
        c = getchar();
        if( c != '\n')while( getchar() != '\n');
        if( c == 'q' || c == 'Q')
          {
          fclose(fp);
          return;
          };
        i =    0;

        };

      };
    fclose(fp);

    };
  pause();
  }
int rnd(x)
int x;
  {
  int r;
  if( x <=1 )
    {
    optlog("rnd() called with bad parameter","space.log");
    return 1;
    };
  r =    rand() % x;
  return(r);

  }
void optlog(string,logue)
char string[],logue[];
  {
  FILE *logf;
  struct tm *p;
  long t;
  char tbuf[26];
  time(&t);
  p = localtime(&t);
  strcpy(tbuf,asctime(p));  /* SAS C library returns 26 char date */
  tbuf[16] = '\0';          /* this gets rid of the newline */
  logf =  fopen(logue,"a");
  if( logf == NULL )
    {
    System_Error("optlog","Unable to output to the logfile");

    };
  fprintf(logf," %s:%s\n",&tbuf[4],string);
  fclose(logf);

  }
