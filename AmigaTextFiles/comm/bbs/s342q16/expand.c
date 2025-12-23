/*
*				expand.c
*
* Expands a msg file.  V.1.2
*/
#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
/*
*				history
*
* 85Nov16 HAW  Modified for MS-DOS.
* 85Apr22 HAW  Move to MS-DOS.
* 84Dec09 HAW  Now merely expands a file, rather than copying & expanding.
* 84Jun22 HAW  Version 1.1 created -- handles any file expansion.
* 84Jun19 HAW  Created.
*/
/*
*				contents
*
*	copy_remainder()	Copies remainder of split msg into file
*	crashout()		irrecoverable error
*	first_part()		Remembers remainder of split msg
*	getUtilNumber()		prompt user for a number, limited range
*	getUtilString()		gets a string from the console
*	main()			Main controller for this program
*/
BPTR	Amsgfl;
extern FILE *upfd;
char		split_buffer[10000];
extern		CONFIG cfg;

char mode[6];

unsigned copy_remainder ( unsigned offset );
void crashout ( char *str );
int first_part ( void );
int getUtilNumber ( char *prompt , unsigned bottom , unsigned top );
void getUtilString ( char *prompt , char *buf , int lim );
void Expand_InitMsgBase ( void );
void SopenFile ( char *filename , BPTR *fd );
long myseek ( BPTR fd , long rpos , int mode );
BPTR myopen ( char *name , char *mode );
void myclose ( BPTR fd );
long myread ( void *buff , int size , int blocks , BPTR fp );
long mywrite ( void *buff , int size , int blocks , BPTR fp );

int NEUtilGetch(void);
int UtilGetch(void);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

/*
* copy_remainder()
*
* This copies the remainder of the split msg onto the "end" of the file.
*/
unsigned copy_remainder(offset)
unsigned offset;
  {
  unsigned char *beg;
  int  count, i;
  beg = split_buffer;
  count = 0;
  printf("Transfer %d sectors to cover split msg\n",
  (offset) ? offset / MSG_SECT_SIZE + 1 : 0);
  while (1)
    {
    if (offset < MSG_SECT_SIZE) break;
    crypte(beg + (MSG_SECT_SIZE * count), MSG_SECT_SIZE, 0);
    if (mywrite(beg + (MSG_SECT_SIZE * count), MSG_SECT_SIZE, 1, Amsgfl) != 1)
      {
      printf("\ncopy_remainder:Error writing first set %d aborting\n",count);
      exit(100);
      };
    count++;
    offset -= MSG_SECT_SIZE;

    }
  if (offset == 0) return (unsigned)count;
  for (i = 0; beg[MSG_SECT_SIZE * count + i] != 255; i++) ;
  for (i++; i < MSG_SECT_SIZE; i++)
  beg[MSG_SECT_SIZE * count + i] = 0;
  crypte(beg + MSG_SECT_SIZE * count, MSG_SECT_SIZE, 0);
  if (mywrite(beg + (MSG_SECT_SIZE * count), MSG_SECT_SIZE, 1, Amsgfl) != 1)
    {
    printf("\ncopy_remainder:Error writing second set %d aborting\n",count);
    exit(100);
    };
  return (unsigned ) (count + 1);

  }
/*
* crashout()
*
* This handles the irrecoverable error.
*/
void crashout(str)
char *str;
  {
  printf(str);
  exit(100);

  }
/*
* first_part()
*
* This remembers the remainder of first msg.
*/
int first_part()
  {
  int  offset, i;
  DATA_BLOCK buf;
  char done;
  offset = 0;
  done   = FALSE;
  do
    {
    if (myread(buf, MSG_SECT_SIZE, 1, Amsgfl) != 1)
      {
      printf("\nError on read first message\n");
      exit(100);
      };
    crypte(buf, MSG_SECT_SIZE, 0);
    i = 0;
    while (buf[i] != 255 && i < MSG_SECT_SIZE)
    split_buffer[offset++] = buf[i++];
    done = !(i == MSG_SECT_SIZE);

    }
  while (!done);
  myseek(Amsgfl, 0, SEEK_SET);  /* rewind to begining */
  return offset;

  }
/*
* getUtilNumber()
*
* This function prompts for a number in (bottom, top) range.
*/
int getUtilNumber(prompt, bottom, top)
char   *prompt;
unsigned bottom;
unsigned top;
  {
  unsigned try;
  char numstring[NAMESIZE];
  do
    {
    getUtilString(prompt, numstring, NAMESIZE);
    try     = atoi(numstring);
    if (try < bottom)  printf("Sorry, must be at least %d\n", bottom);
    if (try > top   )  printf("Sorry, must be no more than %d\n", top);

    }
  while ((try < bottom ||  try > top));
  return (int) try;

  }
/*
* getUtilString()
*
* This will get a string from the user.
*/
void getUtilString(prompt, buf, lim)
char *prompt;
char *buf;
int  lim;       /* max # chars to read */
  {
  char c;
  int  i;
  if (strLen(prompt) > 0)
    {
    printf("\nEnter %s\n : ", prompt, lim);

    }
  i   = 0;
  while (
  c = NEUtilGetch(),
  c	!= NEWLINE
  && c	!= '\r'
  && i     <  lim
  )
    {
    /* handle delete chars: */
    if (c == BACKSPACE)
      {
      putchar(' ');
      putchar(BACKSPACE);
      if (i > 0) i--;
      else
        {
        putchar(' ');
        putchar(BELL);

        }

      }
    else
      {
      putchar(c);
      buf[i++] = c;

      }
    if (i >= lim)
      {
      putchar(BELL);
      putchar(BACKSPACE); i--;

      }

    }
  buf[i]  = '\0';

  }
/*
* main()
*
* This is the main controller.
*/
int  main(void);
int main()
  {
  char     temp[MSG_SECT_SIZE];
  char c;
  int      i, offset,cnt;
  unsigned oldsize, fudge;
  long work;
  strcpy(mode,"rb+");
  printf("Citadel Message expansion program V3.42\n%s\n", COPYRIGHT);
  printf("Have you backed up the ctdlmsg.sys file yet (y/n)? ");
  c = getchar();
  while( getchar() != '\n');
  if ( c != 'Y' && c != 'y' )
    {
    printf("Please do so before proceeding.  Failure to do so may\n");
    printf("result in you loosing everything...\n");
    exit(1);

    }
  printf("\n\nOne moment, please...\n");
  cfg.weAre = UTILITY;
  if (readSysTab(TRUE, TRUE))
    {
    if (access(LOCKFILE, 0) != -1)
      {
      printf("Please do not run Expand using Outside Commands.\n");
      writeSysTab();
      exit(1);

      }
    cfg.weAre = UTILITY;
    Expand_InitMsgBase();
    oldsize     = cfg.maxMSector;
    printf("\nOld size was %dK", cfg.maxMSector / (1024 / MSG_SECT_SIZE));
    cfg.maxMSector = getUtilNumber("\nNew size (in decimal!)",
    (unsigned) cfg.maxMSector/(1024/MSG_SECT_SIZE), 65535);
    cfg.maxMSector *= (1024 / MSG_SECT_SIZE);
    printf("\nThank you.  Working...");
    offset = first_part();
    writeSysTab();	/* Restore again, don't have to reconfigure */
    work = oldsize;
    work *= MSG_SECT_SIZE;
    myseek(Amsgfl, 0,SEEK_END);	/* Get to EOF */
    cnt = 0;
    printf("Starting the expansion\n");
    while (myread(temp, MSG_SECT_SIZE, 1, Amsgfl) == 1)
      {
      printf("reading %08.8d\r",++cnt);
      };
    printf("\n");
    fudge = copy_remainder(offset);
    for (i = 0; i < MSG_SECT_SIZE; i++) temp[i] = 0;
    crypte(temp, MSG_SECT_SIZE, 0);
    i = cfg.maxMSector - oldsize - fudge;
    printf("And now %7d sectors left to initialize\n", i);

    for (; i; i--)
      {
      printf("%7d\r", i);
      if (mywrite(temp, MSG_SECT_SIZE, 1, Amsgfl) != 1)
        {
        printf("****Error writing expansion sectors(%d left to do)\n",i);
        exit(100);
        };
      };
    myclose(Amsgfl);
    printf("\nFinished.  Don't need to reconfigure!  But don't forget");
    printf(" to change CTDLCNFG.SYS.\n ");

    }
  return 0;
  }
/*
* InitMsgBase()
*
* This function opens the msg base(s), inits the msg buffers.
*/
void Expand_InitMsgBase()
  {
  SYS_FILE name;
  makeSysName(name, "ctdlmsg.sys", &cfg.msgArea);
  SopenFile(name, &Amsgfl);
  InitBuffers();

  }
/*
* SopenFile()
*
* This opens one of the .sys files.   Has extra parameter for mode
* which is global.
*/
void SopenFile(char *filename, BPTR *fd)
  {

  /* We use fopen here rather than safeopen for link reasons */
  if ((*fd = myopen(filename, mode)) == NULL)
    {
    printf("?no %s", filename);
    exit(SYSOP_EXIT);

    }

  }

long myseek( BPTR fd, long rpos, int mode)
  {

  switch (mode )
    {
    case SEEK_SET: mode = OFFSET_BEGINNING;break;
    case SEEK_CUR: mode = OFFSET_CURRENT  ;break;
    case SEEK_END: mode = OFFSET_END      ;break;
    default:
    fprintf(stderr,"Error Invalid fseek mode:%d\n",mode);
    return -1;
    };

  if( fd == NULL )
    {
    fprintf(stderr, "Error NULL file pointer\n");
    return -1;
    };
  (void)Seek( fd, rpos, mode );
  return 0;
  }

BPTR myopen( char *name, char *mode )
  {
  return Open( name, MODE_OLDFILE);
  }

void myclose(BPTR fd)
  {
  Close(fd);
  }

long  myread(void *buff, int size, int blocks, BPTR fp    )
  {
  long rsize, isize;
  rsize = size * blocks;
  isize =  Read( fp, buff, rsize  );
  return (isize/size);
  }


long  mywrite(void *buff, int size, int blocks, BPTR fp    )
  {
  long rsize, isize;
  rsize = size * blocks;
  isize =  Write( fp, buff, rsize  );
  return (isize/size);
  }
