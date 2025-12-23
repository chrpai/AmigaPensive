/*

    IO.c
    ----
    Definitions for XPR_IO-functions

    (C) 1990 Oliver Wagner, All Rights Reserved

*/

#include <proto/exec.h>
#include <proto/dos.h>
#include <devices/serial.h>
#include <devices/timer.h>
#include <string.h>

#define print(s) Write(Output(),s,strlen(s))

#include "xproto.h"

void plog(char,char*,...);

/* external Intuition functions */
extern int xpr_opendisplay(void);
extern int xpr_closedisplay(void);
extern int xpr_displayupdate(struct XPR_IO*);
extern int xpr_chk4abort(void);
extern long xpr_windowsig;

/* Global IO-Request for XPR */
extern struct IOExtSer *xpr_serio;

/* external filename array */
extern char *xprfilearray[];

/* static Data */
static struct timerequest treq;
static struct MsgPort *tport;
static char timeropen;

extern short nowindow;

/* The timer functions */
static long opentimer(void)
{
    tport=CreatePort(0,0);
    if(!tport) return(-1);
    treq.tr_node.io_Message.mn_ReplyPort=tport;
    if(OpenDevice("timer.device",UNIT_VBLANK,&treq,0)) return(-1);
    timeropen=1;
    return(0);
}
static void closetimer(void)
{
    if(timeropen) CloseDevice(&treq);
    if(tport) DeletePort(tport);
}
static void qtimer(long micros)
{
    int secs=0;
    if (micros > 1000000) {
	 secs	= micros / 1000000;
	 micros = micros % 1000000;
    }
    treq.tr_time.tv_micro=micros;
    treq.tr_time.tv_secs=secs;
    treq.tr_node.io_Command=TR_ADDREQUEST;
/*    printf("QTIMER s=%ld m=%ld\n",secs,micros);*/
    SendIO(&treq);
}

/* the finfo-function */
static long __saveds xpr_finfo(char *name,long type)
{
    struct FileInfoBlock *fib=AllocMem(sizeof(struct FileInfoBlock),0);
    BPTR lck;
    long result=0;

    plog('+',"FINFO: %s (%ld)\n",name,type);
    if(!fib) return(0);
    if(!(lck=Lock(name,SHARED_LOCK))) goto xit;
    Examine(lck,fib);
    UnLock(lck);
    result=fib->fib_Size;
    if(type==2) result=1;
    else if(type!=1) result=0;
xit:
    FreeMem(fib,sizeof(struct FileInfoBlock));
/*    printf("result=%ld\n",result);*/
    return(result);
}

/* the serwrite function */
static long __saveds xpr_swrite(char *buffer,long size)
{
/*    printf("SWRITE: buffer=%lx size=%ld\n",buffer,size);*/
    xpr_serio->IOSer.io_Length=size;
    xpr_serio->IOSer.io_Data=buffer;
    xpr_serio->IOSer.io_Command=CMD_WRITE;
    DoIO(xpr_serio);
    return((long)xpr_serio->IOSer.io_Error);
}

long chk4cd(void)
{
	xpr_serio->IOSer.io_Command=SDCMD_QUERY;
	DoIO(xpr_serio);
	return(!(xpr_serio->io_Status&(1<<5)));
}

/* the serread function */
static long __saveds xpr_sread(char *buffer,long size,long timeout)
{
    long flag=xpr_windowsig|(1<<xpr_serio->IOSer.io_Message.mn_ReplyPort->mp_SigBit);
    long len,nflag;

    /*printf("SREAD() buffer=%lx size=%ld timeout=%ld\n",buffer,size,timeout);*/
    SetSignal(0,flag);
    if(timeout) {
	flag|=1<<tport->mp_SigBit;
	/*printf("Starting timer...\n");*/
	qtimer(timeout);
    }
    else {
	xpr_serio->IOSer.io_Command=SDCMD_QUERY;
	DoIO(xpr_serio);
	if(!(len=xpr_serio->IOSer.io_Actual)) return(0);
	else {
	    if(len>size) len=size;
	    xpr_serio->IOSer.io_Command=CMD_READ;
	    xpr_serio->IOSer.io_Data=buffer;
	    xpr_serio->IOSer.io_Length=len;
	    DoIO(xpr_serio);
	    /*printf("Fastread return %ld\n",xpr_serio->IOSer.io_Actual);*/
	    return((long)xpr_serio->IOSer.io_Actual);
	}
    }
    SetSignal(0,flag);
    xpr_serio->IOSer.io_Command=CMD_READ;
    xpr_serio->IOSer.io_Data=buffer;
    xpr_serio->IOSer.io_Length=size;
    SendIO(xpr_serio);
    nflag=Wait(flag);
    if(nflag&xpr_windowsig) {
	AbortIO(&treq);
	AbortIO(xpr_serio);
	WaitIO(&treq);
	WaitIO(xpr_serio);
	return(-1);
    }
    if(nflag&(1<<xpr_serio->IOSer.io_Message.mn_ReplyPort->mp_SigBit)) {
	AbortIO(&treq);
	WaitIO(xpr_serio);
	WaitIO(&treq);
	/*printf("Normal read returns %ld (error %ld)\n",xpr_serio->IOSer.io_Actual,(long)xpr_serio->IOSer.io_Error);*/
	return((long)xpr_serio->IOSer.io_Actual);
    }
    /*printf("TIMEOUT:\n");*/
    AbortIO(xpr_serio);
    WaitIO(&treq);
    WaitIO(xpr_serio);
    return((long)xpr_serio->IOSer.io_Actual);
}

/* sflush - flushes serial port */
static long __saveds xpr_sflush(void)
{
    xpr_serio->IOSer.io_Command=CMD_FLUSH;
    DoIO(xpr_serio);
    return((long)xpr_serio->IOSer.io_Error);
}

/* setserial; stolen from TERM by Olsen */
#define ST_PARTYON	(1 << 0)
#define ST_PARTYODD	(1 << 1)
#define ST_7WIRE	(1 << 2)
#define ST_QBREAK	(1 << 3)
#define ST_RADBOOGIE	(1 << 4)
#define ST_SHARED	(1 << 5)
#define ST_EOFMODE	(1 << 6)
#define ST_XDISABLED	(1 << 7)
#define ST_PARTYMARKON	(1 << 8)
#define ST_PARTYMARK	(1 << 9)
#define ST_2BITS	(1 << 10)
#define ST_READ7	(1 << 11)
#define ST_WRITE7	(1 << 12)
static long __saveds xpr_setserial(long Status)
{
	STATIC LONG XprBauds[12] =
	{
		110,
		300,
		1200,
		2400,
		4800,
		9600,
		19200,
		31250,
		38400,
		57600,
		76800,
		115200
	};

	LONG Return,i;

	xpr_serio -> IOSer . io_Command = SDCMD_QUERY;
	DoIO(xpr_serio);

	Return = xpr_serio -> io_SerFlags & 0xFF;

	if(xpr_serio -> io_ExtFlags & SEXTF_MSPON)
		Return |= ST_PARTYMARKON;

	if(xpr_serio -> io_ExtFlags & SEXTF_MARK)
		Return |= ST_PARTYMARK;

	if(xpr_serio -> io_StopBits == 2)
		Return |= ST_2BITS;

	if(xpr_serio -> io_ReadLen == 7)
		Return |= ST_READ7;

	if(xpr_serio -> io_WriteLen == 7)
		Return |= ST_WRITE7;

	for(i = 0 ; i < 12 ; i++)
	{
		if(xpr_serio -> io_Baud == XprBauds[i])
		{
			Return |= (i << 16);

			break;
		}
	}

	if(Status != -1)
	{
		xpr_serio -> IOSer . io_Command	= SDCMD_SETPARAMS;

		xpr_serio -> io_SerFlags		= Status & 0xFF;
		xpr_serio -> io_ExtFlags		= 0;

		if(Status & ST_PARTYMARKON)
			xpr_serio -> io_ExtFlags |= SEXTF_MSPON;

		if(Status & ST_PARTYMARK)
			xpr_serio -> io_ExtFlags |= SEXTF_MARK;

		if(Status & ST_2BITS)
			xpr_serio -> io_StopBits = 2;
		else
			xpr_serio -> io_StopBits = 1;

		if(Status & ST_READ7)
			xpr_serio -> io_ReadLen = 7;
		else
			xpr_serio -> io_ReadLen = 8;

		if(Status & ST_WRITE7)
			xpr_serio -> io_WriteLen = 7;
		else
			xpr_serio -> io_WriteLen = 8;

		DoIO(xpr_serio);

	}

	return(Return);

}
/* EOT [end of theft] */


/* The ffirst() and ffnext() functions */
static long __saveds xpr_ffirst(char *buffer,char *pattern)
{
    strcpy(buffer,xprfilearray[0]);
    plog('+',"FFIRST: returning %s\n",buffer);
    return(1);
}
static long __saveds xpr_fnext(long oc,char *buffer,char *pattern)
{
    if(!xprfilearray[oc]) {
	plog('+',"FNEXT: end of array\n");
	return(0);
    }
    else {
	strcpy(buffer,xprfilearray[oc++]);
	plog('+',"FNEXT: returning %s\n",buffer);
	return(oc);
    }
}

static long __saveds xpr_gets(char*b,char*p)
{
/*    print("GETS: ");print(b);print(" - ");print(p);print("\n");*/
    return(0);
}

/* the stub routines for a4 loading :-( */
static long __saveds xpr_fopen(char*a1,char*a2)
{
    long h;

    plog('+',"FOPEN: %s,'%s'\n",a1,a2);
    switch(*a2) {
	case 'r': return(Open(a1,MODE_OLDFILE));
	case 'w': return(Open(a1,MODE_NEWFILE));
	case 'a': if(h=Open(a1,MODE_OLDFILE)) {
		    Seek(h,0,OFFSET_END);
		    return(h);
		  }
		  return(Open(a1,MODE_NEWFILE));
    }
    return(0);
}
static long __saveds xpr_fclose(long *a1)
{
    if(a1) Close(a1);
    return(0);
}
static long __saveds xpr_fread(char*a1,long a2,long a3,long*fpt)
{
/*    printf("fread(%lx,%ld,%ld,%lx)\n",a1,a2,a3,fpt);*/
    if(!a2) return(0);
    return(Read(fpt,a1,a2*a3));
}
static long __saveds xpr_fwrite(char*a1,long a2,long a3,long*fpt)
{
/*    printf("fwrite(%lx,%ld,%ld,%lx)\n",a1,a2,a3,fpt);*/
    if(!a2) return(0);
    return(Write(fpt,a1,a2*a3));
}
static long __saveds xpr_fseek(long*a1,long a2,long a3)
{
    long h=-1;
/*    printf("fseek(%lx,%ld,%ld)\n",a1,a2,a3);*/

    switch(a3) {
	case 0: h=OFFSET_BEGINNING; break;
	case 1: h=OFFSET_CURRENT; break;
	case 2: h=OFFSET_END; break;
	default: return(-1);
    }
    return((Seek(a1,a2,h)!=-1)?0:-1);
}
static long __saveds xpr_unlink(char*a1)
{
    plog('+',"UNLINK: %s\n",a1);
    return(DeleteFile(a1));
}

/* init the xprIO and build everything */
long init_xpr(struct XPR_IO *IO)
{
   IO->xpr_fopen     = xpr_fopen;
   IO->xpr_fclose    = xpr_fclose;
   IO->xpr_fread     = xpr_fread;
   IO->xpr_fwrite    = xpr_fwrite;
   IO->xpr_sread     = xpr_sread;
   IO->xpr_swrite    = xpr_swrite;
   IO->xpr_sflush    = xpr_sflush;
   IO->xpr_update    = xpr_displayupdate;
   IO->xpr_chkabort  = xpr_chk4abort;
   IO->xpr_setserial = xpr_setserial;
   IO->xpr_ffirst    = xpr_ffirst;
   IO->xpr_fnext     = xpr_fnext;
   IO->xpr_finfo     = xpr_finfo;
   IO->xpr_fseek     = xpr_fseek;
   IO->xpr_gets      = xpr_gets;
   IO->xpr_unlink    = xpr_unlink;
/*   xpr_sflush();*/
   if(xpr_opendisplay()) return(-1);
   return(opentimer());
}

void close_xpr(void)
{
    xpr_closedisplay();
    closetimer();
}
