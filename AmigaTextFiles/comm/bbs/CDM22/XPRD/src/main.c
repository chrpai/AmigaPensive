/*
    XPR
    ---
    eXternal PRotocol Driver

    written & (C) 1990 by Oliver Wagne

*/

#include <proto/dos.h>
#include <proto/exec.h>
#include <devices/serial.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#ifdef V030
#include <exec/execbase.h>
#endif
#include "xpr.h"

extern void sprintf(char*,...);

short os13;

#define print(s) Write(Output(),s,strlen(s))

#define INFOSTRING "XPRD version V1.13; last compiled " __TIME__ " on " __DATE__ "\n\
Written and copyright © 1991 by Oliver Wagner, All Rights Reserved\n\
Contact:\n\x9b33;1m\
	EMail : O.WAGNER@LINK-ESW.ZER (Z-Netz)\n\
		O.WAGNER@LINK-ESW.ZER.SUB.ORG (Internet)\n\
		Oliver Wagner of 2:240/5600 (Wauwaunet)\n\
	Voice : (++49) (0)5659/518\n\
	Data  :	(++49) (0)5659/1307 (24h DST) SUPPORT-BBS\n\
        Snail : Oliver Wagner\n\
                Auf Dem Stein 5\n\
                3441 Weißenborn\n\
		West Germoney\n\x9b0m"

extern char *xprfilearray[];
struct Library *XProtocolBase,*IntuitionBase,*GfxBase;

char *libname="xprzmodem.library";
char *options=0;
char *devname="serial.device";
short devnum=0;
short nodelay=0;
struct Screen *firstscreen=0;
long rbuflen=0;
short sharedaccess=0,rtscts=0;
char *tempdir=0;
long speed=0;
short checkcarrier=0;
short windowx=60,windowy=25;
char *logfile=0;
BPTR logfilep;
char *errorfile=0;
short nowindow=0;

void exit(int);
int atoi(char*);
void initdisplay(void);
long xpr_chk4abort(void);

struct IOExtSer *xpr_serio;
static struct IOExtSer ios;
static struct MsgPort *smp;

static int openserial(char *dev,int unit)
{
    if(!(smp=CreatePort(0,0))) return(-1);
    ios.IOSer.io_Message.mn_ReplyPort=smp;
    if(sharedaccess) ios.io_SerFlags|=SERF_SHARED;
    if(OpenDevice(dev,unit,&ios,0)) {
	DeletePort(smp);
	return(-1);
    }
    xpr_serio=&ios;
    if(rbuflen||speed||rtscts) {
	if(rbuflen) ios.io_RBufLen=rbuflen;
	if(speed) ios.io_Baud=speed;
	if(rtscts) ios.io_SerFlags|=SERF_7WIRE;
	ios.IOSer.io_Command=SDCMD_SETPARAMS;
	DoIO(&ios);
    }
    ios.IOSer.io_Command=SDCMD_QUERY;
    DoIO(&ios);
    speed=ios.io_Baud;
    rtscts=ios.io_SerFlags&SERF_7WIRE;
    return(0);
}
static void closeserial(void)
{
    CloseDevice(&ios);
    DeletePort(smp);
}

static char logbf[160];
void __stdargs plog(char typ,char *string,long arg1,long arg2)
{
	long t;
	if(logfilep) {
		t=time(0);
		sprintf(logbf,"%lc %24.24s : ",(long)typ,ctime(&t));
		Write(logfilep,logbf,strlen(logbf));
		sprintf(logbf,string,arg1,arg2);
		Write(logfilep,logbf,strlen(logbf));
	}
}

int __stdargs closelog(int x)
{
	BPTR f;
	plog('!',"Exiting XPRD\n");
	if(logfilep) Close(logfilep);
	sprintf(logbf,"%-2ld\n",x);
	if(errorfile) {
		if(f=Open(errorfile,MODE_NEWFILE)) {
			Write(f,logbf,3);
			Close(f);
		}
	}		
	return(x);
}
void main(int argc, char**argv)
{
    int c=1,l=0,com,retval=20;
    BPTR templock=0,oldlock=0;
    char *help,ch1,ch2;

    onexit(closelog);
    IntuitionBase=OpenLibrary("intuition.library",36);
    if(!IntuitionBase) {
		IntuitionBase=OldOpenLibrary("intuition.library");
		os13=1;
    }
    GfxBase=OldOpenLibrary("graphics.library");
    print("\x9b32;1m_____________________________________________________________________________\n\n\
 \x9b0;33;3meXternal PRotocol Driver\x9b0m V1.13 by \x9b1mOliver Wagner\x9b0m © 1991, All Rights Reserved\n\
\x9b32;1m_____________________________________________________________________________\n\n\x9b0m");
#ifdef V030
    {
        struct ExecBase *eb=*((struct ExecBase**)4);
        print("\t\t\t68020/30/40 \x9b33mONLY\x9b0m VERSION\n\n");
        if(!(eb->AttnFlags&AFF_68020)) {
		print("\x07");
		exit(20);
	}
    }
#endif
    if(argc<2||*argv[1]=='?') {
	print("Usage: XPRD [-options] s[end]|r[eceive] [file1 [file2 [file3...]]]\navaible options:\n\
-l\x9b3mlibname \x9b0m   - define XPR library (def.: \"xprzmodem.library\")\n\
-o\x9b3moptions \x9b0m   - define XPR startup options string\n\
-d\x9b3mdevname \x9b0m   - define IO device (def.: \"serial.device\")\n\
-u\x9b3munitnum \x9b0m   - define IO device unit (def.: 0)\n\
-x\x9b3m[XS]speed \x9b0m - define IO device baud rate, RTS/CTS, SHARED\n\
-t\x9b3mdirname \x9b0m   - define temporary dir for batch dl\n");
	print("-b\x9b3mbufsize \x9b0m   - define IO device read buffer size (K)\n\
-p\x9b3mx,y \x9b0m       - define window X and Y position\n\
-m\x9b3mlogfile \x9b0m   - enable log option (def.: \"xprd.log\")\n\
-r\x9b3merrfile \x9b0m   - define return code output file\n\
-c	     - enable carrier detect\n\
-n	     - no delay after EOT\n\
-f	     - open window on frontmost screen (DANGER!)\n\
-q	     - don't open window; quiet mode\n\
-i	     - display version and copyright information\n");

	exit(0);
    }
    while(*argv[c]=='-') {
	switch(tolower(argv[c][1])) {
	    case 'l': libname=&argv[c][2]; break;
	    case 'o': options=&argv[c][2]; break;
	    case 'd': devname=&argv[c][2]; break;
	    case 'u': devnum=atoi(&argv[c][2]); break;
	    case 'b': rbuflen=atoi(&argv[c][2])<<10; break;
	    case 'x': if(strlen(argv[c])<3) break;
		      ch1=argv[c][2];
		      ch2=argv[c][3];
		      if(strlen(argv[c])>3) {
				if(ch1=='s'||ch2=='s') sharedaccess=1;
				if(ch1=='x'||ch2=='x') rtscts=1;
				if(ch2=='x'||ch2=='s') help=&argv[c][4];
				else if(ch1=='x'||ch1=='s') help=&argv[c][3];
				else help=&argv[c][2];
		      }
		      else {
				help=&argv[c][2];
				if(ch1=='s') {
					sharedaccess=1;
					help=&argv[c][3];
				}
				else if(ch1=='x') {
					rtscts=1;
					help=&argv[c][3];
				}
		      }
		      speed=atoi(help);
		      break;
	    case 'n': nodelay=1; break;
	    case 'p': help=&argv[c][2];
		      if(*help==',') {
			windowy=atoi(&help[1]);
			break;
		      }
		      windowx=atoi(help);
		      while(*help && *help!=',') help++;
		      if(*help==',') windowy=atoi(&help[1]);
		      break;
   	    case 'f': firstscreen=(struct Screen*)-1; if(strlen(argv[c])==10) stch_l(&argv[c][2],&firstscreen);
		      if(!firstscreen) nowindow=1;
		      break;
	    case 's': sharedaccess=1; break;
	    case 't': tempdir=&argv[c][2]; break;
	    case 'c': checkcarrier=1; break;
	    case 'i': print(INFOSTRING); exit(0);
	    case 'm': if(strlen(argv[c])>2) logfile=&argv[c][2];
		      else logfile="XPRD.log";
		      if(!(logfilep=Open(logfile,MODE_OLDFILE))) {
			if(!(logfilep=Open(logfile,MODE_NEWFILE))) {
			  print("Unable to open logfile!\x07\n");
			  exit(20);
			}
		      }
		      else Seek(logfilep,0,OFFSET_END);
		      plog('!',"Started XPRD\n");
		      break;
	    case 'r': errorfile=&argv[c][2]; break;
	    case 'q': nowindow=1; break;
	    default: print("Illegal option: '");print(&argv[c][1]);
		     print("'!\x07\n"); exit(20);
	}
	c++;
	if(c==argc) {
	    print("Missing SEND or RECEIVE!\n\x07");
	    exit(20);
	}
    }
    if(tolower(*argv[c])!='s'&&tolower(*argv[c])!='r') {
	print("Missing SEND or RECEIVE!\n\x07");
	exit(20);
    }
    com=(tolower(*argv[c])=='s')?1:0;
    c++;
    if(c==argc) xprfilearray[l++]="NAPFSUELZE";
    else while(c!=argc) xprfilearray[l++]=argv[c++];
    xprfilearray[l]=0;

    if(!(XProtocolBase=OldOpenLibrary(libname))) {
	print("Failed to open XPR library!\n\x07");
	exit(20);
    }
    if(openserial(devname,devnum)) {
	print("Failed to open IO device!\n\x07");
	CloseLibrary(XProtocolBase);
	exit(20);
    }
    if(xprinit(options)) {
	print("Failed to init XPR library!\n\x07");
	CloseLibrary(XProtocolBase);
	closeserial();
	exit(20);
    }
    if(tempdir) {
	if(!(templock=Lock(tempdir,SHARED_LOCK))) {
	    print("Failed to Lock() tempory dir!\n\x07");
	    goto xit;
	}
	if(!(oldlock=CurrentDir(templock))) {
	    print("Can't CurrentDir() to temporary dir!\n\x07");
	    goto xit;
	}
    }
    if(nowindow) print("Doing transfer in quiet mode\n");
    initdisplay();
    if(!xpr_chk4abort()) {
	retval=(((com)?xprsend():xprreceive())==1)?0:20;
    }
    if(!nodelay) Delay(136);
xit:
    if(oldlock) CurrentDir(oldlock);
    if(templock) UnLock(templock);
    xprclose();
    closeserial();
    CloseLibrary(XProtocolBase);
    exit(retval);
}
