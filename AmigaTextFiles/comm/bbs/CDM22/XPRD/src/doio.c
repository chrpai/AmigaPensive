/*
    xprdoio.c
    ---------
    routines for sending and receiving files
*/

#include "xproto.h"
#include "xpragma.h"

/* !!! set up by caller !!! */
extern struct IOExtSer *xpr_serio;
extern struct Library *XProtocolBase;

/* fill in before calling!!! */
char *xprfilearray[256];

static struct XPR_IO xio;

void plog(char,char*,...);

extern int init_xpr(struct XPR_IO*);
extern void close_xpr(void);

int xprinit(char *options)
{
    if(init_xpr(&xio)) {
	close_xpr();
	return(-1);
    }
    xio.xpr_filename=options;
    if(!XProtocolSetup(&xio)) {
	close_xpr();
	return(-1);
    }
    return(0);
}

int xprsend(void)
{
    int r;
    plog('!',"SEND\n");
    xio.xpr_filename=xprfilearray[0];
    r=XProtocolSend(&xio);
    plog('!',"SEND returns %lx\n",r);
    return(r);
}

int xprreceive(void)
{
    int r;
    plog('!',"RECEIVE\n");
    xio.xpr_filename=xprfilearray[0];
    r=XProtocolReceive(&xio);
    plog('!',"RECEIVE returns %lx\n",r);
    return(r);
}

void xprclose(void)
{
    XProtocolCleanup(&xio);
    close_xpr();
}
