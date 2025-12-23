#define TITLE   "Citadel User Log statistics "

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

extern CONFIG    cfg;            /* A buncha variables           */
extern logBuffer logBuf;         /* Pippul buffer                */
extern FILE             *logfl;         /* log file descriptor          */
extern LogTable    *logTab;
SListBase MailForward = { NULL, CheckFwd, NULL, NULL, EatForwarding };

char dodebug = FALSE;

void Analyze(void);
void Display(void);
FILE *fopen();

int Floors = 0,
    Lfmask = 0,
    Expert = 0,
    Aide = 0,
    Time = 0,
    Oldtoo = 0,
    Net = 0,
    entry,
    hi = 0,
    low = 0,
    Forward = 0,
    Nulls = 0,
    ECD = 0,
    Halfdup = 0;

/*
 * main()
 *
 * Main manager.
 */
int main(int ,char **);
int main(argc,argv)
int  argc;
char **argv;
{
    int i;
    SYS_FILE fn;
    extern char *READ_ANY;

    cfg.weAre = UTILITY;
    printf("%s %s\n%s\n",TITLE, VERSION_NAME, COPYRIGHT);
    if (!readSysTab(FALSE, TRUE)) exit(1);
    makeSysName(fn, "ctdllog.sys", &cfg.logArea);
    if ((logfl = fopen(fn, READ_ANY)) == NULL) {
        printf("Can't open the Citadel log!\n");
        exit(1);
    }
    initLogBuf(&logBuf);
    makeSysName(fn, "ctdlfwd.sys", &cfg.roomArea);
    MakeList(&MailForward, fn, NULL);

    entry = 0;
    for ( i = 0; i < cfg.MAXLOGTAB; i++ ) {
        getLog(&logBuf,i);
        fprintf(stderr, "log #%-4d\r",i);
        if (logBuf.lbflags.L_INUSE) {
            entry++;
            Analyze();
        }
    }
    Display();
  return 0;
}

/*
 * Analyze()
 *
 * This analyzes an account.
 */
void Analyze()
{
    if (logBuf.lbflags.FLOORS) Floors++;
    if (logBuf.lbflags.LFMASK) Lfmask++;
    if (logBuf.lbflags.EXPERT) Expert++;
    if (logBuf.lbflags.AIDE) Aide++;
    if (logBuf.lbflags.TIME) Time++;
    if (logBuf.lbflags.OLDTOO) Oldtoo++;
    if (logBuf.lbflags.NET_PRIVS) Net++;
    if (logBuf.lbflags.HALF_DUP) Halfdup++;
    if (logBuf.lbnulls != 0) Nulls++;
    if (logBuf.lbdelay != 0) ECD++;
    if (SearchList(&MailForward, logBuf.lbname) != NULL)
        Forward++;
    if (logBuf.lbwidth > 41) hi++;

    else low++;
}

/*
 * Display()
 *
 * Final statistics display.
 */
void Display()
{
    printf("%d accounts in use\n\n", entry);
    printf("%d are using Floor Mode\n", Floors);
    printf("%d have Net privs\n", Net);
    printf("%d use Mail Forwarding\n", Forward);
    printf("%d are Experts\n", Expert);
    printf("%d are Aides\n", Aide);
    printf("%d use Half-duplex mode\n", Halfdup);
    printf("%d use LineFeeds\n", Lfmask);
    printf("%d display the time on messages\n", Time);
    printf("%d display last old on new request\n", Oldtoo);
    printf("%d use more than 0 nulls\n", Nulls);
    printf("%d users have column widths greater than 41, %d less than\n",
                        hi, low);
    printf("%d use .ECD\n", ECD);
}

/*
 * crashout()
 *
 * Handles fatal errors.
 */
void crashout(str)
char *str;
{
    exit(printf(str));
}

/*
 * CheckFwd()
 *
 * forward mail to whom?
 */
void *CheckFwd(ForwardMail *data, char *who)
{
    return (strCmpU(data->UserName, who) == SAMESTRING) ? data : NULL;
}

/*
 * EatForwarding()
 *
 * This will eat a line from ctdlfwd.sys
 * Format: <username><tab><system spec><tab><alias>
 * where "system spec" can be a domain-name.
 * "alias" can be the same as username
 */
void *EatForwarding(char *line)
{
    ForwardMail *data;
    char *tab;

    data = GetDynamic(sizeof *data);
    tab = strchr(line, '\t');
    *tab++ = 0;
    data->UserName = strdup(line);
    line = tab;
    tab = strchr(line, '\t');
    *tab++ = 0;
    data->System = strdup(line);
    data->Alias = strdup(tab);
    return data;
}

