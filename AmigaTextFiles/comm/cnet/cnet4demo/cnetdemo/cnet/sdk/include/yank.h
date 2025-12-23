
#ifndef CNET_YANK
#define CNET_YANK

struct	YankData {
	long	nparts, parts[10][2];
	long	nsets, set[10][2];
	long	maint, SysMaint;

	short	Age, TimeOnLine, br, mx;
	short	listn, parent0;
	struct	UserData user1;

	short	rbyid, rtoid;
	UBYTE	rtome,	rfree,	rfavorite, runval, rprivate,
		rbrandnew, rnresponse, rpass, rsince, rnew,
		rchron, rbyme, runtil, rpreview;
	UBYTE	qwk, TextSet;
	struct	IsDate	rdate, udate;
	char	rsearch[64], rpattok[204];

	char	keyword[8][32],
		filter [8][32];

	short	sorder[240];

	char	gmPath[128];	// full path to _mhead4 & _mtext4 folder files
};

struct	YankType {
	struct	YankType *next;

	ULONG	yid;
	short	id, parm;

	long	nmess, ymess;

	struct	IsDate	Submitted;
	UBYTE	status, globreak;
};


#endif
