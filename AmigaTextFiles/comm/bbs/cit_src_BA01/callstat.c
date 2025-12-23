
/*
Hue: This is the source of the current version of callstat.  I've been
running it on my system for some time, and it seems to be stable.
Unfortunately, one of the other Swarm Sysops, who has a hard-drive-
based STadel node, has reported that this version of callstat acts
very strangely with his system.  I haven't yet figured where the
problem is...
In any case, the basic logic is sound, and fairly straightforward.
The program is undocumented as yet; I haven't reached the point
where I'm ready to release it to the public, and I never document
my own code...until it's too late.  :-)  Anyway, I don't expect
you'll have much trouble figuring things out.  The only problem
areas are in the Cit-specific stuff, such as access to ctdltabl.sys
and calllog.sys.
Any comments, questions, complaints, or speeches would be welcomed,
unless they deal with my personal programming style.  :-)
--Royce, 13Mar88, far too late in the evening...
*/
#define BOOLEAN int
#define CITADEL86       1
/* #define ATARI_ST        1 */
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
#ifdef ATARI_ST
#include <osbind.h>
#include "audit.h"
extern struct config    cfg;
#endif
#ifdef CITADEL86
#define safeopen        fopen
#define Cconin()        getchar()
#define Fdelete         unlink
#define doExit          exit
typedef char pathSpec[200];
extern CONFIG           cfg;
void get_data(void);
void munch(void);
void write_data(void);
void write_stat_file(void);
void UpdateBar(void);
long baud_code(long baud);
void file_error(char *file, char *access);
BOOLEAN find_on(void);
char get_stuff(void);
void write_a_stat(char *desc, char stat);
void write_hist_bar(unsigned stat, float factor);
#endif
struct
  {
  unsigned hour,min;

  }
on,off,last;
BOOLEAN newuser,termstay,disconnect,preempt,timeout,evile,nosysop,update,docb,
debug, barupdate, SysUp = FALSE;
char    ch,date[20],str[20];
long baud;
unsigned long dur;
FILE    *safeopen(),*fp;
pathSpec fnlog = "",fndat,fnstat,fncallbaud;
long total,calls[24],durations[20],callbaud[24][9],
nnewusers,ntermstays,ndisconnects,nnewdiscons,npreempts,
ntimeouts,neviles;
long bauds[9] =
  {
  0, 0, 0,  0, 0, 0,  0, 0, 0

  };
long    totalduration,totalidle, netcount;
long    baudduration[9] =
  {
  0l, 0l, 0l,  0l, 0l, 0l,  0l, 0l, 0l

  };
char    bartitle[100] = "";
int main(int , char **);
int main(argc,argv)
int     argc;
char    *argv[];
  {
  cfg.weAre = UTILITY;
  printf("Citadel Caller Log Statistics Report, %s\n", VERSION_NAME);
  printf("Do a 'callstat -?' for help.\n");
  nosysop = FALSE;
  update = FALSE;
  docb = FALSE;
  debug = FALSE;
  while (argc > 1)
    {
    --argc;
    ++argv;
    #ifndef CITADEL86
    if (strcmp(*argv, "-c") == 0)
    docb = TRUE;
    if (strcmp(*argv, "-d") == 0)
    debug = TRUE;
    if (strcmp(*argv, "-s") == 0)
    nosysop = TRUE;
    if (strcmp(*argv, "-u") == 0)
    update = TRUE;
    if (strcmp(*argv, "-?") == 0)
      {
      printf("usage: callstat [-c] [-s] [-u]\n");
      printf("       -c writes callbaud.sys\n");
      printf("       -s turns off sysConsole stats\n");
      printf("       -u deletes calllog.sys & updates calldata.sys\n");
      doExit(0);

      }
    #else
    if (strcmp(*argv, "-c") == 0)
    docb = TRUE;
    else if (strcmp(*argv, "-d") == 0)
    debug = TRUE;
    else if (strcmp(*argv, "-s") == 0)
    nosysop = TRUE;
    else if (strcmp(*argv, "-u") == 0)
    update = TRUE;
    else if (strncmp(*argv, "-b", 2) == 0)
      {
      barupdate = TRUE;
      if (strlen(*argv) > 2)
      strcpy(bartitle, *argv + 2);

      }
    else if (strcmp(*argv, "-?") == 0)
      {
      printf("usage: callstat [-c] [-s] [-u] [filename]\n");
      printf("       -c writes callbaud.sys\n");
      printf("       -s turns off sysConsole stats\n");
      printf("       -u deletes calllog.sys & updates calldata.sys\n");
      printf("       -b[title name] updates bar.sys\n");
      printf("       \"filename\" analyze this file, instead\n");
      doExit(0);

      }
    else if (**argv != '-')
      {
      strcpy(fnlog, *argv);

      }
    #endif

    }
  #ifdef ATARI_ST
  if (!readSysTab(FALSE))
  #endif
  #ifdef CITADEL86
  if (!readSysTab(FALSE, TRUE))
  #endif
  doExit(1);
  #ifdef ATARI_ST
  if (((cfg.call_log & aEXIT) == 0) || ((cfg.call_log & aLOGIN) == 0) ||
  ((cfg.call_log & aDNLOAD == 1)))
    {
    printf("Sorry, but callstat currently needs system ups & downs\n");
    printf("and user logins to be recorded, and file transfers to\n");
    printf("be unrecorded.\n");
    doExit(1);

    }
  #endif
  #ifdef ATARI_ST
  if (strlen(fnlog) == 0)
  sprintf(fnlog,"%s\\CALLLOG.SYS",&cfg.codeBuf[cfg.auditDir]);
  sprintf(fndat,"%s\\CALLDATA.SYS",&cfg.codeBuf[cfg.sysDir]);
  sprintf(fnstat,"%s\\CALLSTAT.SYS",&cfg.codeBuf[cfg.auditDir]);
  sprintf(fncallbaud,"%s\\CALLBAUD.SYS",&cfg.codeBuf[cfg.auditDir]);
  #endif
  #ifdef CITADEL86
  if (strlen(fnlog) == 0)
  makeSysName(fnlog, "calllog.sys", &cfg.auditArea);
  makeSysName(fndat, "calldata.sys", &cfg.auditArea);
  makeSysName(fnstat, "callstat.sys", &cfg.auditArea);
  makeSysName(fncallbaud, "callbaud.sys", &cfg.auditArea);
  #endif
  last.hour = 100;
  get_data();
  munch();
  if (update)
    {
    Fdelete(fnlog);
    write_data();

    }
  #ifdef CITADEL86
  if (barupdate)
    {
    UpdateBar();

    }
  #endif
  write_stat_file();
  return 0;

  }
void file_error(char *file, char *access)
  {
  printf("Can't open %s for %s!\n",file,access);
  printf("Hit return: "); /* For people running under GEM desktop. */
  ch = Cconin();
  printf("\n");

  }
void get_data()
  {
  unsigned        i,j;
  if ((fp = safeopen(fndat,"r")) == NULL)
    {
    date[0] = NULL;
    total = 0;
    zero_struct(calls);
    zero_struct(durations);
    totalduration = 0;
    totalidle = 0;
    zero_struct(bauds);
    zero_struct(callbaud);
    nnewusers = 0;
    ntermstays = 0;
    ndisconnects = 0;
    nnewdiscons = 0;
    npreempts = 0;
    ntimeouts = 0;
    neviles = 0;

    }
  else
    {
    printf("Reading cumulative data...\n");
    fscanf(fp,"%s\n",date);
    fscanf(fp,"%d\n",&total);
    for (i = 0; i <= 23; i++)
    fscanf(fp,"%d\n",&calls[i]);
    for (i = 0; i <= 19; i++)
    fscanf(fp,"%d\n",&durations[i]);
    fscanf(fp,"%ld\n",&totalduration);
    fscanf(fp,"%ld\n",&totalidle);
    for (i = 0; i <= 8; i++)
    fscanf(fp,"%d\n",&bauds[i]);
    for (i = 0; i <= 23; i++)
    for (j = 0; j <= 8; j++)
    fscanf(fp,"%d\n",&callbaud[i][j]);
    fscanf(fp,"%d\n",&nnewusers);
    fscanf(fp,"%d\n",&ntermstays);
    fscanf(fp,"%d\n",&ndisconnects);
    fscanf(fp,"%d\n",&nnewdiscons);
    fscanf(fp,"%d\n",&npreempts);
    fscanf(fp,"%d\n",&ntimeouts);
    fscanf(fp,"%d\n",&neviles);
    fclose(fp);

    }

  }
long baud_code(long baud)
  {
  if (baud == 300)            return 1;
  else if (baud == 1200)      return 2;
  else if (baud == 2400)      return 3;
  else if (baud == 4800)      return 4;
  else if (baud == 9600)      return 5;
  else if (baud == 14400)     return 6;
  else if (baud == 19200)     return 7;
  else if (baud >= 38400)     return 8;
  else  return 0;

  }
char isnet;
BOOLEAN find_on()       /* Scan for next user login time. */
  {
  int     rc;
  char    up, buffer[800];
  buffer[0] = 0;
  do
    {
    str[0] = NULL;
    rc = fscanf(fp,"%s ",str);
    if (debug)                     printf("-%s-",str);
    if (strcmp(str, "up") == 0)    up = TRUE;
    /* Next if handles system down/up. */
    if( strlen(str) + strlen(buffer) > sizeof(buffer) )
      {
      printf("Possible corruption in calllog.sys at --%s--\n", str);
      buffer[0] = 0;
      };
    strcat(buffer, str);
    if ((rc == 1) && (str[0] == '@'))
      {
      if (debug)
      printf("\n");
      if (last.hour != 100)
        {
        if (SysUp && up)
          {
          /* damaged calllog */
          fscanf(fp,"%d:",&last.hour);
          fscanf(fp,"%d\n",&last.min);
          on.hour = 100;

          }
        else
          {
          fscanf(fp,"%d:",&on.hour);
          fscanf(fp,"%d\n",&on.min);

          }

        }
      else
        {
        fscanf(fp,"%d:",&last.hour);
        fscanf(fp,"%d\n",&last.min);
        on.hour = 100;

        }
      if (on.hour != 100)
        {
        if (last.hour > on.hour)
        on.hour += 24;
        dur = ((on.hour * 60 + on.min) - (last.hour * 60 + last.min));
        /*printf("\n1: adding %d\n", dur);*/
        totalidle += dur;
        last.hour = 100;

        }
      up = FALSE;

      }

    }
  while (strcmp(str, "mode:")
    && (rc != EOF) && ((str[0] != ':') || (str[1] != NULL)));
  if (debug)  printf("buffer is -%s-", buffer);
  /*
  Bug: Normally, the only time the substring " : " shows up in the
  calllog, is between a userID and the date of the login.  Thus
  the previous while loop scans for words (i.e. a sequence of
  characters terminated by white-space), and terminates when it
  finds the word ": ".  Normally, this means that the date and
  time of signon are to immediately follow.
  However, if some userID contains the substring " : " (or ": "
  at the start), callstat will rapidly go to heg.  More
  stringent error-checking is needed to guard against this
  (e.g., check to make sure that something of the form ddCccdd
  follows the current instance of ": ", where d is a digit and
  C/c is a character)...
  */
  if (debug)  printf("\n");
  if (rc == EOF)
    return FALSE;
  else
    {
    SysUp = TRUE;
    fscanf(fp,"%s ",str);   /* Skip the date */
    if (date[0] == NULL)    /* Record it if needed (first time) */
    strcpy(date,str);
    isnet = (strcmp(buffer, "Systeminnetworkmode:") == 0);
    return TRUE;

    }

  }
char get_stuff()
  {
  int     rc;
  fscanf(fp,"%d:",&on.hour);
  fscanf(fp,"%d - ",&on.min);
  rc = fscanf(fp,"%d:",&off.hour);
  if (rc != 1)
    {
    printf("\nYuck!  Your calllog.sys file is corrupted!\n");
    printf("(A user is recorded as logging in, but not out.)\n");
    printf("Please use an editor to fix things before rerunning.\n");
    fclose(fp);
    doExit(1);

    }
  fscanf(fp, (isnet) ? "%d" : "%d (", &off.min);
  if (!isnet)
    {
    rc = fscanf(fp,"%d)",&baud);
    if (rc == 0)
      {
      /* Login was at sysConsole */
      baud = 0;
      fscanf(fp,"%s)",str);

      }
    baud = baud_code(baud);

    }
  else baud = 0;
  newuser = FALSE;
  termstay = FALSE;
  disconnect = FALSE;
  preempt = FALSE;
  timeout = FALSE;
  evile = FALSE;
  do
    {
    ch = getc(fp);
    switch (ch)
      {
      case '+' :
      newuser = TRUE;
      break;
      case '-' :
      termstay = TRUE;
      break;
      case 'd' :
      disconnect = TRUE;
      break;
      case 'p' :
      preempt = TRUE;
      break;
      case 't' :
      timeout = TRUE;
      break;
      case 'E' :
      evile = TRUE;
      default :
      break;

      }

    }
  while (ch != '\n');
  return isnet;

  }
void munch()
  {
  char net;
  if ((fp = safeopen(fnlog,"r")) == NULL)
    {
    file_error("calllog.sys","input");
    return;

    }
  printf("Munching...\n");
  while (find_on())
    {
    net = get_stuff();
    if (!net) total++;
    printf("\r%d",total);
    if (!net) calls[on.hour]++;
    if (last.hour > on.hour)
    on.hour += 24;
    dur=((on.hour*60+on.min)-(last.hour*60+last.min));
    /*printf("\n2: adding %ld (Last was %d:%02d, now is %d:%02d)\n", dur, last.hour, last.min, on.hour, on.min);*/
    totalidle += dur;
    if (on.hour >= 24)
    on.hour -= 24;
    last.hour = off.hour;
    last.min = off.min;
    if (on.hour > off.hour)
    off.hour += 24;
    dur=((off.hour*60+off.min) - (on.hour * 60 + on.min));
    if (!net)
      {
      totalduration += dur;
      baudduration[baud] += dur;

      }
    else netcount++;
    dur /= 15;
    if (dur > 19)
    dur = 19;
    if (!net)
      {
      durations[dur]++;
      bauds[baud]++;
      callbaud[on.hour][baud]++;
      if (newuser)
      nnewusers++;
      if (termstay)
      ntermstays++;
      if (disconnect)
        {
        ndisconnects++;
        if (newuser)
        nnewdiscons++;

        }
      if (preempt)
      npreempts++;
      if (timeout)
      ntimeouts++;
      if (evile)
      neviles++;

      }

    }
  fclose(fp);
  printf("\n");

  }
void write_data()
  {
  unsigned        i,j;
  if ((fp = safeopen(fndat,"w")) == NULL)
  file_error("calldata.sys","output");
  else
    {
    printf("Writing cumulative data...\n");
    fprintf(fp,"%s\n",date);
    fprintf(fp,"%d\n",total);
    for (i = 0; i <= 23; i++)
    fprintf(fp,"%d\n",calls[i]);
    for (i = 0; i <= 19; i++)
    fprintf(fp,"%d\n",durations[i]);
    fprintf(fp,"%ld\n",totalduration);
    fprintf(fp,"%ld\n",totalidle);
    for (i = 0; i <= 8; i++)
    fprintf(fp,"%d\n",bauds[i]);
    for (i = 0; i <= 23; i++)
    for (j = 0; j <= 8; j++)
    fprintf(fp,"%d\n",callbaud[i][j]);
    fprintf(fp,"%d\n",nnewusers);
    fprintf(fp,"%d\n",ntermstays);
    fprintf(fp,"%d\n",ndisconnects);
    fprintf(fp,"%d\n",nnewdiscons);
    fprintf(fp,"%d\n",npreempts);
    fprintf(fp,"%d\n",ntimeouts);
    fprintf(fp,"%d\n",neviles);
    fclose(fp);

    }

  }
void write_hist_bar(unsigned stat, float factor)
  {
  unsigned        i;
  fprintf(fp,": ");
  if ((stat / factor) >= 1.0)
  for (i = 1; i <= (int) (stat / factor); i++)
  fprintf(fp,"*");
  else if (stat >= 1)
  fprintf(fp,"*");
  fprintf(fp," (%d)\n", stat);
  fflush(fp);

  }
void write_a_stat(char *desc, char stat)
  {
  #ifndef CITADEL86
  fprintf(fp,"%s: %d (%d%%)\n",desc,stat,stat * 100 / total);
  #else
  fprintf(fp, "%-30s%-15d%d\n", desc, stat, stat * 100 / total);
  #endif

  }
void write_stat_file()
  {
  unsigned        i,j,max;
  float           factor;
  Fdelete(fnstat);
  if ((fp = safeopen(fnstat,"w")) == NULL)
  file_error("callstat.sys","output");
  else
    {
    printf("Writing stat file...\n");
    fprintf(fp,"\n The Antithesystem Memorial RT Histogram.\n\n");
    fprintf(fp," The following statistics have been collected since %s.\n\n",date);
    fprintf(fp," Total number of calls represented: %d\n\n",total);
    max = 25;
    for (i = 0; i <= 23; i++)
    if (calls[i] > max)
    max = calls[i];
    factor = max / 25.0;
    fprintf(fp," Usage histogram (each number represents an hour of the day, each '*'\n");
    fprintf(fp,"represents about %d calls at that hour):\n",(int) factor);
    for (i = 0; i <= 23; i++)
      {
      fprintf(fp,"%3d",i);
      write_hist_bar(calls[i],factor);

      }
    fprintf(fp,"\n");
    max = 25;
    for (i = 0; i <= 19; i++)
    if (durations[i] > max)
    max = durations[i];
    factor = max / 25.0;
    fprintf(fp," Call duration histogram (each number represents a call duration of at\n");
    fprintf(fp,"most that much time, each '*' represents about %d calls of that duration):\n",(int) factor);
    for (i = 0; i <= 19; i++)
      {
      fprintf(fp,"%2d:%d",(i + 1) * 15 / 60,(i + 1) * 15 % 60);
      if (i % 4 == 3)
      fprintf(fp,"0");
      write_hist_bar(durations[i],factor);

      }
    fprintf(fp," The average call duration is about %ld minutes.\n",totalduration / total);
    fprintf(fp," The average idle time before/after a call is about %ld minutes.\n\n",totalidle / total);
    max = 25;
    for (i = 0; i <= 8; i++)
    if (bauds[i] > max)
    max = bauds[i];
    factor = max / 25.0;
    fprintf(fp," Baud rate histogram (each number represents a baud rate, each '*' represents\n");
    fprintf(fp,"about %d calls at that baud rate):\n",(int) factor);
    for (i = 0 + nosysop; i <= cfg.sysBaud + 1; i++)
      {
      switch (i)
        {
        case 0 :  fprintf(fp,"  sys");  break;
        case 1 :  fprintf(fp,"  300");  break;
        case 2 :  fprintf(fp," 1200");  break;
        case 3 :  fprintf(fp," 2400");  break;
        case 4 :  fprintf(fp," 4800");  break;
        case 5 :  fprintf(fp," 9600");  break;
        case 6 :  fprintf(fp,"14400");  break;
        case 7 :  fprintf(fp,"19200");  break;
        case 8 :  fprintf(fp,"38400");  break;

        }
      write_hist_bar(bauds[i],factor);

      }
    fprintf(fp,"\n");
    fprintf(fp, "There were %ld network sessions.\n\n", netcount);
    #ifndef CITADEL86
    write_a_stat(" Number of new users",nnewusers);
    write_a_stat(" Number of uses of .T(erminate) S(tay)",ntermstays);
    write_a_stat(" Number of disconnects",ndisconnects);
    write_a_stat(" Number of new user disconnects",nnewdiscons);
    write_a_stat(" Number of preemptions",npreempts);
    write_a_stat(" Number of time-outs",ntimeouts);
    write_a_stat(" Number of EVILE users",neviles);
    #else
    fprintf(fp, "%-25s%-15s%s\n", " Description",
    "Total Number", "Percentage");
    fprintf(fp, "%-25s%-15s%s\n", " -----------",
    "------------", "----------");
    write_a_stat(" New users", nnewusers);
    write_a_stat(" .T(erminate) S(tay) usage", ntermstays);
    write_a_stat(" Disconnects", ndisconnects);
    write_a_stat(" New user disconnects", nnewdiscons);
    write_a_stat(" Preemptions", npreempts);
    write_a_stat(" Time-outs", ntimeouts);
    write_a_stat(" EVILE users", neviles);
    #endif
    fclose(fp);

    }
  if (docb != TRUE)
  return;
  Fdelete(fncallbaud);
  if ((fp = safeopen(fncallbaud,"w")) == NULL)
  file_error("callbaud.sys","output");
  else
    {
    printf("Writing call/baud file...\n");
    fprintf(fp,"\n");
    fprintf(fp," The following statistics have been collected since %s.\n\n",date);
    fprintf(fp," Total number of calls represented: %d\n\n",total);
    max = 25;
    for (i = 0; i <= 23; i++)
    for (j = 0; j <= 8; j++)
    if (callbaud[i][j] > max)
    max = callbaud[i][j];
    factor = max / 25.0;
    fprintf(fp," Usage/baud rate histogram (each pair of numbers represents an hour of the\n");
    fprintf(fp,"day and a baud rate, each '*' represents about %d calls at that hour and\n",(int) factor);
    fprintf(fp,"baud rate):\n");
    for (i = 0; i <= 23; i++)
    for (j = 0 + nosysop; j <= cfg.sysBaud + 1; j++)
      {
      fprintf(fp,"%3d,",i);
      switch (j)
        {
        case 0 :  fprintf(fp,"  sys");  break;
        case 1 :  fprintf(fp,"  300");  break;
        case 2 :  fprintf(fp," 1200");  break;
        case 3 :  fprintf(fp," 2400");  break;
        case 4 :  fprintf(fp," 4800");  break;
        case 5 :  fprintf(fp," 9600");  break;
        case 6 :  fprintf(fp,"14400");  break;
        case 7 :  fprintf(fp,"19200");  break;
        case 8 :  fprintf(fp,"38400");  break;

        }
      write_hist_bar(callbaud[i][j],factor);

      }
    fclose(fp);

    }

  }
void crashout(str)
char *str;
  {
  puts(str);
  doExit(1);

  }
/*
* Note Royce doesn't support 4800, so we must force in a 0.
*/
void UpdateBar()
  {
  FILE *fd, *fopen();
  extern char *APPEND_TEXT;
  if ((fd = fopen("bar.sys", APPEND_TEXT)) != NULL)
    {
    if (strlen(bartitle) == 0)
      {
      printf("Title for this data: ");
      gets(bartitle);

      }
    fprintf(fd, "%s\n", bartitle);
    fprintf(fd, "%d\n", bauds[0]);
    fprintf(fd, "%ld\n", baudduration[0]);
    fprintf(fd, "%d\n", bauds[1]);
    fprintf(fd, "%ld\n", baudduration[1]);
    fprintf(fd, "%d\n", bauds[2]);
    fprintf(fd, "%ld\n", baudduration[2]);
    fprintf(fd, "%d\n", bauds[3]);
    fprintf(fd, "%ld\n", baudduration[3]);
    fprintf(fd, "%d\n", bauds[4]);
    fprintf(fd, "%ld\n", baudduration[4]);
    fprintf(fd, "%d\n", bauds[5]);
    fprintf(fd, "%ld\n", baudduration[5]);
    fprintf(fd, "%d\n", bauds[6]);
    fprintf(fd, "%ld\n", baudduration[6]);
    fprintf(fd, "%d\n", bauds[7]);
    fprintf(fd, "%ld\n", baudduration[7]);
    fprintf(fd, "%d\n", bauds[8]);
    fprintf(fd, "%ld\n", baudduration[8]);
    fprintf(fd, "%d\n", 0);
    fprintf(fd, "0\n");
    fprintf(fd, "%d\n", bauds[8]);
    fprintf(fd, "%ld\n", baudduration[8]);
    fprintf(fd, "%ld\n", totalduration);
    fprintf(fd, "%ld\n", totalidle);
    fprintf(fd, "%d\n", nnewusers);
    fprintf(fd, "\n");
    fclose(fd);

    }

  }
