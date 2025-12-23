/*
*				sysutil.c
*
* This is the repository of most of the commonly used system dependent code
* in the Citadel utilities.
*/
/*
*				history
*
* 87Feb12 HAW  Created.
*/
#define SYSTEM_DEPENDENT
#include "ctdl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdarg.h"
#include "fcntl.h"
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
#include "stdarg.h"
#include "dos.h"
#include "sysutil.h"

char bigbuffer[MAXTEXT];


/*
*				Contents
*
*	getUtilDate()		gets date from system for utilities
*	strCmpU()		strcmp(), but ignoring case distinctions
*	UtilGetch()		gets a character
*	NEUtilGetch()		gets a character, no echo
*/
char *READ_ANY="rb";
char *WRITE_ANY="wb";
char *R_W_ANY="r+b";
char *W_R_ANY="w+b";
char *APPEND_TEXT="a";
char *READ_TEXT="r";
char *WRITE_TEXT="w";
char *APPEND_ANY  = "a+b";
char  *monthTab[13] =
  {
  "", "Jan", "Feb", "Mar",
  "Apr", "May", "Jun",
  "Jul", "Aug", "Sep",
  "Oct", "Nov", "Dec"

  };

void totalBytes(long *size, FILE *fd)
  {
  long oldpos;                  /* old position of this file */
  oldpos = ftell(fd);           /* save old position */
  fseek(fd,0l,SEEK_END);       /* go to end of file */
  *size = ftell(fd);            /* get current position */
  fseek(fd,oldpos,SEEK_SET);    /* back to begining */
  }

/*
* UtilGetch()
*
* This will get a character from console.
*/
int UtilGetch()
  {
  int c;
  c = getchar();
  putchar(c == '\r' ? '\n' : c);
  return c;

  }
/*
* NEUtilGetch()
*
* This will get a character from console, non-echo mode.
*/
int NEUtilGetch()
  {
  int c;
  c = getchar();
  return c;

  }
/*
* safeopen()
*
* This opens a file.
*/
FILE *safeopen(name, mode)
char *name, *mode;
  {
  return fopen(name, mode);

  }
char *mtab[] =
  {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"

  };
/*
* LastOn()
*
* This function will return a human string representing the given date.
*/
char *LastOn(long lastdate, char shortstyle)
  {
  struct tm *data;
  static char buffer[40];
  char *m;
  /* 0l represents Never in our scheme */
  if (lastdate == 0l) return "Never";
  data = localtime(&lastdate);
  if (!shortstyle)
      {
    civTime(&data->tm_hour, &m);
    sprintf(buffer, "%d%s%02d @ %d:%02d %s", data->tm_year,
    mtab[data->tm_mon], data->tm_mday, data->tm_hour, data->tm_min, m);

    }
  else
  sprintf(buffer, "%d%s%02d", data->tm_year, mtab[data->tm_mon],
  data->tm_mday);
  return buffer;

  }
/*
* civTime()
*
* This converts military time to Civilian time.
*/
void civTime(int *hours, char **which)
  {
  if (*hours >= 12)
  *which = "pm";
  else
  *which = "am";
  if (*hours >= 13)
  *hours -= 12;
  if (*hours == 0)
  *hours = 12;

  }

/*
 * ChkNtoStr()
 *
 * This is a required testing function for list handling.  Returns pointer to
 * char.
 */
void *ChkNtoStr(NumToString *d1, NumToString *d2)
{
    if (d1->num == d2->num) return d1->string;
    return NULL;
}

/*
 * FreeNtoStr()
 *
 * This frees a data node for archiving for later use.
 */
void FreeNtoStr(NumToString *d)
{
    free(d->string);
    free(d);
}

/*
 * EatNMapStr()
 *
 * This Eats a string formatted as "<digit> <string>" for list handling.
 */
void *EatNMapStr(char *line)
{
    char    *s;

    if ((s = strchr(line, ' ')) == NULL)
	return NULL;

    return NtoStrInit(atoi(line), s+1, 0, FALSE);
}

/*
* NtoStrInit()
*
* This is a utility function to generate temporary or permanent data nodes
* for searching or adding to Archive-type lists.
*/
NumToString t =    {    0, NULL    };
void *NtoStrInit(int num, char *str, int num2, char needstatic)
  {
  NumToString *temp;
  if (needstatic)
    {
    temp = &t;
    if (temp->string != NULL) free(temp->string);

    }
  else  temp = (NumToString *) GetDynamic(sizeof *temp);
  temp->num  = num;
  temp->num2 = num2;
  temp->string = strdup(str);
  return (void *) temp;

  }
/*
 * putFLChar()
 *
 * This is used to upload files.
 * returns: ERROR on problems else TRUE.
 */
FILE *upfd;

int putFLChar(int c)
{
    if (fputc(c, upfd) != EOF)	return TRUE;
    /* else */			printf("Write error: %d\n", ferror(upfd));
				return ERROR;
}


int (*ToFileWork)(int s) = putFLChar;
/************************************************************************/
/*      ToFile() sends the data to file pointed at by upfd              */
/************************************************************************/
void ToFile(char *format, ...)
  {
  va_list argptr;
  int i;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  for (i = 0; bigbuffer[i]; i++) (*ToFileWork)(bigbuffer[i]);
  (*ToFileWork)(0);      /* Send NULL since it did before */

  }
void Do_Stack_Check(void);
void Do_Stack_Check() {}
void splitF(FILE*fp, char *format, ...) {}
