/* Citadel Internet Mail reader V1.00
** this routine will scan a directory specified in arg1
** Each entry is expected to be a mail message.
** The subject is tested against arg2(if found)
**
*/
#include <libraries/dos.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <proto/dos.h>
#include "sysdep.h"

extern int _OSERR;
char mode_flag;    /* U - uuencoded mail, B - Base 64 */
char debug_flag;   /* V - verbose, D - full debug */

int Process_File(char *mail, char *subject, char *dump, char *processed);
int main(int,char **);
int Do_Break(void);
char *Find_Text(FILE *fp, char *text);

int main( argc, argv)
int argc;     /* should be 4 */
char *argv[]; /* current location */
  {
  int status;
  printf("Citadel Internet Mail Reader %s\n",VERSION_NAME);

  if( onbreak(&Do_Break) )printf("Cannot set break for ^C");

  if( argc != 5 && argc != 6 && argc != 7 )
    {
    printf("Please setup this program with the proper arguments.\n");
    printf("CIMR arg1 arg2 arg3 arg4 [arg5]\n");
    printf("   arg1: This is the mail full path name such as cit:mail/*.am\n");
    printf("         which is where your mailer stores mail messages\n");
    printf("   arg2: This is the subject header for the messages to file\n");
    printf("         to search for.\n");
    printf("   arg3: Where to dump the uuencoded message(full pathname)\n");
    printf("   arg4: What do we do after processing\n");
    printf("   arg5: Mode, U - uuencoded(default), or B - Base64\n");
    printf("   arg6: Debug Mode - D, full debug or V, verbose mode\n");
    printf(" This program will read the files specified in arg1, find the subject\n");
    printf(" header specified in arg2 dumping the uuencoded portion(begin to end)\n");
    printf(" into arg3, then save the used message (arg4=save) or remove it(arg4=remove\n");
    printf("\n");
    printf(" Status returned is 0 if message found\n");
    printf("                    5 if message not found\n");
    printf("                   20 if error occured\n");
    status = 20;
    }
  else
    {
    debug_flag = ' ';
    mode_flag  = 'U';  /* default is uuencoded */
    if( argc >= 6 )
      {
      switch ( argv[5][0] )
        {
        case 'u': /* uuencoded */
        case 'U': mode_flag = 'U'; break;
        case 'b': /* base 64 */
        case 'B': mode_flag = 'B'; break;
        default: printf("Error: Invalid mode specifed:%s\n",argv[5]);
        };
      if( argc == 7 )
        {
        switch (argv[6][0])
          {
          case 'd':  /* full debug */
          case 'D':  debug_flag = 'D'; break;
          case 'v':  /* verbose mode */
          case 'V':  debug_flag = 'V'; break;
          default: printf("Error: invalid debug mode specified:%s\n",argv[6]);
          };
        };
      };
    if( debug_flag == 'D' )
      {
      printf("  mail path: %s\n",argv[1]);
      printf("    subject: %s\n",argv[2]);
      printf("destination: %s\n",argv[3]);
      printf(" resolution: %s\n",argv[4]);
      printf("       mode: %s\n",argv[5]);
      };
    status = Process_File(argv[1], argv[2], argv[3], argv[4]);
    };
  return status;
  }

int Process_File(char *mail, char *subject, char *dump, char *processed)
  {
  FILE *fp;
  FILE *op;
  struct FileInfoBlock *info;
  int error,attr;
  char *ptr, *tptr;
  char *dir;
  char fullname[128];
  if( debug_flag == 'V' || debug_flag == 'D')
    {
    printf("Process_File(%s, %s, %s, %s)\n", mail, subject, dump, processed);
    };
  dir = strdup(mail);
  ptr = &dir[strlen(dir)-1];
  while( *ptr != '/' && *ptr != ':' && ptr != dir)ptr--;
  ptr[1] = '\0';
  info  = (struct FileInfoBlock *)calloc(1,sizeof(struct FileInfoBlock));
  if( info == NULL )
    {
    printf("Unable to get memory for FileInfoBlock, aborting\n");
    return 20;
    };

  attr = 1; /* find all files not directories */
  error = dfind(info,mail,attr);  /* get first one*/
  while( error == 0 )
    {
    /**
      For each file, read the lines until a subject is found and
      process the subject.
    **/
    if( debug_flag == 'V' || debug_flag == 'D')printf("processing %s\n",info->fib_FileName);
    strcpy(fullname,dir);
    strcat(fullname,info->fib_FileName);
    if(  (fp = fopen(fullname,"r")) == NULL )
      {
      printf("unable to open %s, skipping it...\n", fullname);
      }
    else
      {
      if( ( ptr = Find_Text(fp,"subject")) != NULL )
        {
        tptr = &ptr[9];            /* start of subject */
        if( debug_flag == 'D') printf("Found:%s\n",ptr);
        if( strnicmp(tptr,subject,strlen(subject)) == 0)
          {
          char *string;
          free(ptr);
          string = (mode_flag == 'U') ? "begin 644" : "Content-Transfer-Encoding: base64";
          if( ( ptr = Find_Text(fp,string) ) != NULL )
            {
            if( debug_flag == 'D') printf("Found:%s\n",ptr);
            if( ( op=fopen(dump,"w")) == NULL )
              {
              printf("unable to open output file:%s\n",dump);
              free(ptr);
              return 20;
              }
            else
              {
              char line[80];
              /* copy the file to the output */
              if( mode_flag == 'B' )
                {  /* the base64 "header" stuff */
                while( fgets(line,sizeof(line), fp) )
                  {
                  if( strncmp(line,"Content-",8) == 0 )continue;
                  if( line[0] == '\n' )continue;
                  break;
                  };
                fputs(line,op); /* write first line of base64 data */
                }
              else
                {
                fputs(ptr,op);  /* write first line for uuencode data */
                };
              free(ptr);
              while( fgets(line,sizeof(line),fp) )fputs(line,op);
              fclose(op);
              fclose(fp);
              /*
               check processed for "save" or "remove"
              */
              if( stricmp(processed,"remove") == 0 )
                {
                printf("deleting %s\n",fullname);
                remove(fullname);
                }
              else if( stricmp(processed,"save") != 0 )
                {
                printf(" Invalid option, should be save or remove was %s\n",processed);
                printf(" assumed save\n");
                };
              return 0;   /* success! */
              };
            };
          }
        else
          {
          free(ptr);
          fclose(fp);
          };
        }
      else
        {
        free(ptr);
        };
      fclose(fp);
      };
    error = dnext(info);
    };

  return 5;
  }

char *Find_Text(FILE *fp, char *text)
  {
  char line[80];  /* assume lines are 80 char or less */
  int  len;
  /**
    read the file processing each line until we find "text"
    as the first thing on the line.
  **/
  if( debug_flag == 'D') printf("Searching for:%s\n",text);
  len = strlen(text);
  while( fgets(line,sizeof(line),fp) )
    {
    if( debug_flag == 'D') printf("%s is text found\n",line);
    if( strnicmp(text,line,len) == 0 )
      {
      return strdup(line);
      };
    };
  return NULL;
  }

int Do_Break()
  {
  printf("\n ^C - ***Break***\n ");
  (void)fcloseall();
  return(1);

  }
