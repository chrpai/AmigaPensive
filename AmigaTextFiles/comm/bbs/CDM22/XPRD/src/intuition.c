
/*
    Intuition.c
    -----------
    Intuition functions for XPR

*/

#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <string.h>
#include "xproto.h"
#include <intuition/intuitionbase.h>

static int fontoffset;

extern void getcdname(char*);
extern short os13;
extern short nowindow;

void plog(char,char*,...);
void printf(char*,...);

extern short checkcarrier,windowx,windowy;
extern long chk4cd(void);

extern void sprintf(char*,char*,);

extern struct Screen *firstscreen;
extern struct IntuitionBase *IntuitionBase;

long xpr_windowsig;
static struct Window *w;
static struct RastPort *rp;
static struct NewWindow nw={0,0,520,136,
    2,1,
    CLOSEWINDOW,
    WINDOWDRAG|WINDOWDEPTH|WINDOWCLOSE|RMBTRAP,
    0,0,0,0,0,0,0,0,0,
    WBENCHSCREEN};

static void print(int x,int y,char *s)
{
    char buff[52];
    Move(rp,8+(x<<3),15+(y<<3)+fontoffset);
    sprintf(buff,"%-50.50s",s);
    Text(rp,buff,50);
}
static void xprint(int x,int y,char *s)
{
    Move(rp,8+(x<<3),15+(y<<3)+fontoffset);
    Text(rp,s,strlen(s));
}
static void nprint(int x,int y,int v)
{
    char buff[8];
    if(v==-1) return;
    sprintf(buff,"%-7ld",v);
    Move(rp,8+(x<<3),15+(y<<3)+fontoffset);
    Text(rp,buff,7);
}

/* Opens the window */
static struct TextAttr top80={"topaz.font",8,0,0};
int xpr_opendisplay(void)
{
    if(nowindow) return(0);
    if((firstscreen==(struct Screen*)-1)) firstscreen=IntuitionBase->FirstScreen;
    if(firstscreen) {
	nw.Type=firstscreen->Flags&SCREENTYPE;
	nw.Screen=firstscreen;
    }
    if(windowy<0) windowy=0;
    if(windowx<0) windowx=0;
    if(!os13) {
	struct Screen *fscr=firstscreen;

	if(!fscr) fscr=LockPubScreen(0);
	if(fscr) {
		fontoffset=fscr->BarHeight-10;
		if(!firstscreen) UnlockPubScreen(0,fscr);
	}
    }	

    nw.Height+=fontoffset;

    if(firstscreen) {
        if(windowy+nw.Height>firstscreen->Height) windowy=firstscreen->Height-nw.Height-1;
        if(windowx+520>firstscreen->Width) windowx=firstscreen->Width-521;
    }
    nw.TopEdge=windowy;
    nw.LeftEdge=windowx;
    if(!(w=OpenWindow(&nw))) return(-1);
    xpr_windowsig=1<<(w->UserPort->mp_SigBit);
    SetFont(rp=w->RPort,OpenFont(&top80));
    SetWindowTitles(w,"XPR Transmission Status Window",
	"eXternal PRotocol Driver V1.13, Written & © 1991 by Oliver Wagner");
    SetDrMd(rp,JAM2);
    SetAPen(rp,2);
    Move(rp,8,92+fontoffset);
    Draw(rp,510,92+fontoffset);
    SetAPen(rp,3);
    xprint(2,1,"Protocol:");
    xprint(2,2,"Filename:");
    xprint(2,3,"Filesize:");
    xprint(3,4,"Message:");
    xprint(1,5,"Last Err.:");
    xprint(4,6,"Blocks:");
    xprint(28,6,"Blocksize:");
    xprint(5,7,"Bytes:");
    xprint(31,7,"Errors:");
    xprint(3,8,"Elapsed:");
    xprint(29,8,"Expected:");
    xprint(2,9,"Datarate:");
    xprint(29,9,"Blockchk:");

    xprint(4,11,"Device:");
    xprint(1,12,"BPS/Flags:");
    xprint(8,13,"CD:");
    xprint(6,14,"Path:");    

    SetAPen(rp,1);
    return(0);
}

extern char *devname;
extern char *tempdir;
extern short devnum,checkcarrier;
extern short sharedaccess,rtscts;
extern int speed;

void initdisplay(void)
{
        char buff[256];
	if(nowindow) return;
	sprintf(buff,"%-.30s (%ld)",devname,(long)devnum);
	xprint(12,11,buff);
	sprintf(buff,"%ld %s%s",speed,(sharedaccess)?"(SHARED) ":"",(rtscts)?"(RTS/CTS)":"");
	xprint(12,12,buff);
        xprint(12,13,(checkcarrier)?"enabled":"disabled");
	getcdname(buff);
        xprint(12,14,buff);
}

/* Closes the Window ;-) */
void xpr_closedisplay(void)
{
    if(w) CloseWindow(w);
}

/* Makes Displayupdate */
void __saveds xpr_displayupdate(struct XPR_UPDATE *xu)
{
    long ud=xu->xpru_updatemask;

    if(nowindow) return;
    if(ud&XPRU_PROTOCOL) print(12,1,xu->xpru_protocol);
    if(ud&XPRU_FILENAME) print(12,2,xu->xpru_filename);
    if(ud&XPRU_FILESIZE) nprint(12,3,xu->xpru_filesize);
    if(ud&XPRU_MSG) {
	plog('+',"MSG: %s\n",xu->xpru_msg);
	print(12,4,xu->xpru_msg);
    }
    if(ud&XPRU_ERRORMSG) {
	plog('+',"ERROR: %s\n",xu->xpru_errormsg);
	print(12,5,xu->xpru_errormsg);
    }
    if(ud&XPRU_BLOCKS) nprint(12,6,xu->xpru_blocks);
    if(ud&XPRU_BLOCKSIZE) nprint(39,6,xu->xpru_blocksize);
    if(ud&XPRU_BYTES) nprint(12,7,xu->xpru_bytes);
    if(ud&XPRU_ERRORS) nprint(39,7,xu->xpru_errors);
    if(ud&XPRU_ELAPSEDTIME) xprint(12,8,xu->xpru_elapsedtime);
    if(ud&XPRU_EXPECTTIME) xprint(39,8,xu->xpru_expecttime);
    if(ud&XPRU_DATARATE) nprint(12,9,xu->xpru_datarate);
    if(ud&XPRU_BLOCKCHECK) xprint(39,9,xu->xpru_blockcheck);
}

/* Checks for User Abort */
long __saveds xpr_chk4abort(void)
{
    struct IntuiMessage *im=0;
    long ret=0;

    if(!nowindow) im=GetMsg(w->UserPort);

    if(checkcarrier) {
        if(!chk4cd()) {
		ret=-1;
		if(!nowindow) xprint(12,13," ");
	}
    }
    if(!im) return(ret);  
    ReplyMsg(im);
    return(-1);
}
