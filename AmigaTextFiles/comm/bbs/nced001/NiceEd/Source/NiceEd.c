/*
** NiceEd - Usereditor for TransAmiga
**  Public Domain 1993 by Adam Sjoegren
*/

#define VER "0.01"

const char version[] = "\0$VER: NiceEd "VER" ("__DATE__") "__TIME__" asj\0Call Interstellar Navigation * +45 31354775";

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <exec/exec.h>

#include <dos/dostags.h>
#include <dos/exall.h>
#include <dos/dos.h>

#include <graphics/displayinfo.h>

#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>

#include <libraries/reqtools.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/reqtools.h>

#include "NiceEd.h"
#include "NiceEd_protos.h"

#define DEFAULT_DIRNAME "BBS:Users/"
#define DEFAULT_SYSTEMDIRNAME "BBS:System/"

extern struct GfxBase		*GfxBase;
extern struct IntuitionBase *IntuitionBase;
extern struct Library		*GadToolsBase;
struct ReqToolsBase			*ReqToolsBase;
struct Screen	*screen;
struct Window	*window;
struct Gadget	*glist;
struct Gadget	*name, *usernumber, *kbdl, *kbul,
				*handle, *fidl, *fiul,
				*street, *birth, *lastcall,
				*computer, *msgs, *calls,
				*postalcode, *scrlength, *netmail,
				*city, *command, *graphics,
				*province, *editor, *help,
				*phone,
				*password, *flags[8],
				*accesslevel, *ratio, *timelimit, *unused,
				*next, *prev, *save, *choose,
				*text, *add, *new, *normal;
APTR vi;
int allusers=0;

#define NAME		0
#define USERNUMBER	1
#define KBDL 		2
#define KBUL		3
#define HANDLE 		4
#define FIDL 		5
#define FIUL		6
#define STREET 		7
#define BIRTH 		8
#define LASTCALL	9
#define COMPUTER 	10
#define MSGS 		11
#define CALLS		12
#define POSTALCODE 	13
#define SCRLENGTH 	14
#define NETMAIL		15
#define CITY 		16
#define COMMAND 	17	/* Must be in order! */
#define GRAPHICS	18	/**/
#define EDITOR 		19	/**/
#define HELP		20	/* EOO */
#define PROVINCE 	21
#define PASSWORD 	22
#define FLAGS0		23	/* Must be in order! */
#define FLAGS1		24	/**/
#define FLAGS2		25	/**/
#define FLAGS3		26	/**/
#define FLAGS4		27	/**/
#define FLAGS5		28	/**/
#define FLAGS6		29	/**/
#define FLAGS7		30	/* EOO */
#define ACCESSLEVEL	31
#define RATIO 		32
#define TIMELIMIT 	33
#define UNUSED		34
#define PREV		35
#define NEXT		36
#define SAVE		37
#define CHOOSE		38
#define TEXT		39
#define PHONE		40
#define NEW			41
#define NORMAL		42

#define sprintf rawsprintf

void main(int argc, char *argv[])
{
	char *dirname = DEFAULT_DIRNAME;
	char *systemdirname = DEFAULT_SYSTEMDIRNAME;
	struct List *userlist;
	
	if( argc>1 )
	{
		dirname = argv[1];
		if( argc>2 )
		{
			systemdirname=argv[2];
		}
	}

	if( !(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary(REQTOOLSNAME, 38L)) )
	{
		PutStr("*** Error NiceEd: Couldn't open ReqTools.library 38+\n");
		exit(20);
	}

	if( userlist=scanusers(dirname) )
	{
		showusers(userlist, dirname, systemdirname);
		freelist(userlist);
	}

	if( ReqToolsBase ) CloseLibrary((struct Library *)ReqToolsBase);

	exit(0);
}

#define BUFSIZE 32768L

struct List *scanusers(char *dirname)
{
	struct ExAllControl *eacon;
	struct ExAllData *eadat;
	struct List *userlist=NULL;
	BPTR baselock;
	char *buffer;
	BOOL more;
	
	if( buffer=(char *)AllocVec(BUFSIZE, MEMF_CLEAR) )
	{
		if( userlist=(struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR) )
		{
			NewList(userlist);
			userlist->lh_Type = NT_USER;
	
			if( eacon=AllocDosObject(DOS_EXALLCONTROL, NULL) )
			{
				if( baselock=Lock(dirname, ACCESS_READ) )
				{
					eacon->eac_LastKey=0;
					eacon->eac_MatchString=NULL;
					eacon->eac_MatchFunc=NULL;

					do
					{
						more=ExAll(baselock, (struct ExAllData *)buffer, BUFSIZE, ED_TYPE, eacon);
				
						if( (!more) && (IoErr() != ERROR_NO_MORE_ENTRIES) )
						{
							PutStr("\n***ERROR: NiceEd - ExAll() error!\n");
						}
				
						if( eacon->eac_Entries == 0 )
							continue;
				
						eadat = (struct ExAllData *) buffer;
						do
						{
							if( eadat->ed_Type < 0 )	/* FT_FILE ! */
							{
								if( 0!=strnicmp(eadat->ed_Name, "users.bbs", 9) && eadat->ed_Name[0]!='@' )
								{
									if( !addnode(userlist, eadat->ed_Name) )
										PutStr("*** addnode failed :-(\n");
									allusers++;
								}
							}
		
							eadat=eadat->ed_Next;
						} while( eadat );
		
					} while( more );

					UnLock(baselock);
				}

				FreeDosObject(DOS_EXALLCONTROL, eacon);
			}	
			else
			{
				FreeVec(userlist);
			}
		}

		FreeVec(buffer);
	}
	else
	{
		return(NULL);
	}

	return(userlist);
}

BOOL addnode(struct List *list, char *name)
{
	struct Node *newnode;

	if( !(newnode=(struct Node *)AllocVec(sizeof(struct Node), MEMF_CLEAR)) )
		return(FALSE);

	newnode->ln_Succ=newnode->ln_Pred=NULL;
	newnode->ln_Type = NT_USER;
	newnode->ln_Pri = 255-ToUpper(name[0]);
	newnode->ln_Name = strdup(name);

	Enqueue(list, newnode);

	return(TRUE);
}

void freelist(struct List *list)
{
	struct Node *remnode;

	while( !(IsListEmpty(list)) )
	{
		remnode=RemHead(list);
		freenode(remnode);
	}

	FreeVec(list);
}

void freenode(struct Node *node)
{
	if( node )
	{
		if( node->ln_Name ) free(node->ln_Name );
		FreeVec(node);
	}
}

#define WINHEIGHT (640)
#define WINWIDTH  (200)
#define LABEL (screen->RastPort.TxWidth*10)
#define NUMLABEL (screen->RastPort.TxWidth*10)
#define CHECKGADWIDTH	(26)
#define TEXTGADWIDTH (screen->RastPort.TxWidth*30)+4
#define NUMGADWIDTH (screen->RastPort.TxWidth*7)+4
#define CYCLEGADWIDTH (NUMLABEL+NUMGADWIDTH)
#define GADHEIGHT (screen->WBorTop+screen->BarHeight+2)

#define LEFT_COL1 (screen->WBorLeft+1)+LABEL
#define LEFT_COL2 LEFT_COL1+(TEXTGADWIDTH+LABEL+1)
#define LEFT_COL3 LEFT_COL2+(NUMGADWIDTH+NUMLABEL+1)
#define LEFT_COL4 LEFT_COL2+2*(NUMGADWIDTH+NUMLABEL+1)

#define TOP_ROW1 (screen->WBorTop+screen->BarHeight)
#define TOP_ROW2 (TOP_ROW1)+GADHEIGHT+1
#define TOP_ROW3 (TOP_ROW1)+2*(GADHEIGHT+1)
#define TOP_ROW4 (TOP_ROW1)+3*(GADHEIGHT+1)
#define TOP_ROW5 (TOP_ROW1)+4*(GADHEIGHT+1)
#define TOP_ROW6 (TOP_ROW1)+5*(GADHEIGHT+1)
#define TOP_ROW7 (TOP_ROW1)+6*(GADHEIGHT+1)
#define TOP_ROW8 (TOP_ROW1)+7*(GADHEIGHT+1)
#define TOP_ROW9 (TOP_ROW1)+8*(GADHEIGHT+1)
#define TOP_ROW10 (TOP_ROW1)+9*(GADHEIGHT+1)
#define TOP_ROW11 (TOP_ROW1)+10*(GADHEIGHT+1)
#define TOP_ROW12 (TOP_ROW1)+11*(GADHEIGHT+1)

#define LEFT_NUMCOL1 (screen->WBorLeft+1)+NUMLABEL
#define LEFT_NUMCOL2 LEFT_NUMCOL1+(NUMGADWIDTH+NUMLABEL+1)
#define LEFT_NUMCOL3 LEFT_NUMCOL1+2*(NUMGADWIDTH+NUMLABEL+1)
#define LEFT_NUMCOL4 LEFT_NUMCOL1+3*(NUMGADWIDTH+NUMLABEL+1)

static char ScreenTitle[] =	"NiceEd - Donated to the Public Domain 1993 by Adam Sjoegren";
static char Title[]		  = "NiceEd v"VER" - The UserEditor for TransAmiga";


struct Window *getwindow(void)
{
	struct NewGadget ng;
	struct Gadget *gad;

	static char *commandlist[]	= { "Stacking", "Hotkeys", NULL, };
	static char *graphicslist[]	= { "ASCII",	"ANSI", NULL, };
	static char *editorlist[]	= { "Line-ed",	"Full-screen", NULL, };
	static char *helplist[]		= { "Expert",	"Novice", NULL, };

	static WORD zoom[4] = {0,11,300,11};


	screen=LockPubScreen(NULL);

	if( !(vi=GetVisualInfo(screen, TAG_END, TAG_END)) )
		PutStr("No VisualInfo!\n");
		
	zoom[2] = 20*screen->RastPort.TxWidth;
	glist=NULL;
	gad=CreateContext(&glist);

	ng.ng_LeftEdge	 = LEFT_COL2-LABEL;
	ng.ng_TopEdge	 = TOP_ROW10;
	ng.ng_Width      = CYCLEGADWIDTH;
	ng.ng_Height     = GADHEIGHT;
	ng.ng_GadgetText = "New";
	ng.ng_TextAttr   = screen->Font;
	ng.ng_GadgetID   = NEW;
	ng.ng_Flags		 = PLACETEXT_IN;
	ng.ng_VisualInfo = vi;
	ng.ng_UserData	 = NULL;

	new=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	new->UserData=(APTR)BUTTON_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3-LABEL;
	ng.ng_GadgetText = "Normal (10)";
	ng.ng_GadgetID		= NORMAL;

	normal=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	normal->UserData=(APTR)BUTTON_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3-LABEL;
	ng.ng_TopEdge	 = TOP_ROW11;
	ng.ng_GadgetText = "Next _>";
	ng.ng_GadgetID   = NEXT;

	next=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	next->UserData=(APTR)BUTTON_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2-LABEL;
	ng.ng_GadgetText = "_< Previous";
	ng.ng_GadgetID		= PREV;

	prev=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	prev->UserData=(APTR)BUTTON_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2-LABEL;
	ng.ng_TopEdge	 = TOP_ROW9;
	ng.ng_GadgetText = "Choose user";
	ng.ng_Height     = GADHEIGHT;
	ng.ng_GadgetID	 = CHOOSE;

	choose=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	choose->UserData=(APTR)BUTTON_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3-LABEL;
	ng.ng_TopEdge	 = TOP_ROW9;
	ng.ng_GadgetText = "_Save user";
	ng.ng_GadgetID	 = SAVE;
	ng.ng_Height     = GADHEIGHT;

	save=gad=CreateGadget(BUTTON_KIND, gad, &ng,
		GT_Underscore,	'_',
		GA_Disabled,	FALSE,
		TAG_END);
	save->UserData=(APTR)BUTTON_KIND;


	ng.ng_LeftEdge   = LEFT_NUMCOL2;
	ng.ng_TopEdge    = TOP_ROW11;
	ng.ng_Width      = NUMGADWIDTH;
	ng.ng_GadgetText = "_Unused:";
	ng.ng_GadgetID   = UNUSED;
	ng.ng_Flags		 = PLACETEXT_LEFT;

	unused=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	unused->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge   = LEFT_NUMCOL1;
	ng.ng_GadgetText = "Ti_me:";
	ng.ng_GadgetID   = TIMELIMIT;

	timelimit=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	timelimit->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge   = LEFT_NUMCOL2;
	ng.ng_TopEdge    = TOP_ROW10;
	ng.ng_GadgetText = "Rat_io:";
	ng.ng_GadgetID   = RATIO;

	ratio=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	ratio->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge   = LEFT_COL1;
	ng.ng_GadgetText = "Acc_ess:";
	ng.ng_GadgetID   = ACCESSLEVEL;

	accesslevel=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	accesslevel->UserData=(APTR)INTEGER_KIND;

	ng.ng_TopEdge	 = TOP_ROW9;
	ng.ng_GadgetText = "P_assword:";
	ng.ng_GadgetID   = PASSWORD;
	ng.ng_Width      = TEXTGADWIDTH;

	password=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	11,
		TAG_END, TAG_END);
	password->UserData=(APTR)STRING_KIND;

	ng.ng_TopEdge	 = TOP_ROW8;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_GadgetText = "_Computer:";
	ng.ng_GadgetID   = COMPUTER;

	computer=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	computer->UserData=(APTR)STRING_KIND;

	ng.ng_TopEdge	 = TOP_ROW7;
	ng.ng_GadgetText = "P_rovince:";
	ng.ng_GadgetID   = PROVINCE;

	province=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	province->UserData=(APTR)STRING_KIND;

	ng.ng_TopEdge	 = TOP_ROW6;
	ng.ng_GadgetText = "City:";
	ng.ng_GadgetID   = CITY;

	city=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	city->UserData=(APTR)STRING_KIND;

	ng.ng_TopEdge	 = TOP_ROW5;
	ng.ng_GadgetText = "P_ost:";
	ng.ng_GadgetID   = POSTALCODE;

	postalcode=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	postalcode->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2;
	ng.ng_GadgetText = "Len_gth:";
	ng.ng_GadgetID   = SCRLENGTH;
	ng.ng_Width      = NUMGADWIDTH;

	scrlength=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	scrlength->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3;
	ng.ng_GadgetText = "Netmail:";
	ng.ng_GadgetID   = NETMAIL;

	netmail=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	netmail->UserData=(APTR)INTEGER_KIND;

/**/

	ng.ng_LeftEdge	 = LEFT_COL1;
	ng.ng_TopEdge	 = TOP_ROW4;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_GadgetText = "S_treet:";
	ng.ng_GadgetID   = STREET;

	street=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	street->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2;
	ng.ng_Width      = NUMGADWIDTH;
	ng.ng_GadgetText = "Msgs:";
	ng.ng_GadgetID   = MSGS;

	msgs=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	msgs->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3;
	ng.ng_GadgetText = "Calls:";
	ng.ng_GadgetID   = CALLS;

	calls=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	calls->UserData=(APTR)INTEGER_KIND;

/**/

	ng.ng_LeftEdge	 = LEFT_COL1;
	ng.ng_TopEdge	 = TOP_ROW3;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_GadgetText = "_Phone:";
	ng.ng_GadgetID   = PHONE;

	phone=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	phone->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2;
	ng.ng_Width      = NUMGADWIDTH;
	ng.ng_GadgetText = "_Birth:";
	ng.ng_GadgetID   = BIRTH;

	birth=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	6,
		TAG_END, TAG_END);
	birth->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3;
	ng.ng_GadgetText = "Last:";
	ng.ng_GadgetID   = LASTCALL;

	lastcall=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	6,
		TAG_END, TAG_END);
	lastcall->UserData=(APTR)STRING_KIND;

/**/

	ng.ng_LeftEdge	 = LEFT_COL1;
	ng.ng_TopEdge	 = TOP_ROW2;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_GadgetText = "_Handle:";
	ng.ng_GadgetID   = HANDLE;

	handle=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	handle->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2;
	ng.ng_Width      = NUMGADWIDTH;
	ng.ng_GadgetText = "_Files DL:";
	ng.ng_GadgetID   = FIDL;

	fidl=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	fidl->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3;
	ng.ng_GadgetText = "Files UL:";
	ng.ng_GadgetID   = FIUL;

	fiul=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	fiul->UserData=(APTR)INTEGER_KIND;

/**/

	ng.ng_LeftEdge	 = LEFT_COL1;
	ng.ng_TopEdge	 = TOP_ROW1;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_GadgetText = "_Name:";
	ng.ng_GadgetID   = NAME;

	name=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	name->UserData=(APTR)STRING_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2;
	ng.ng_Width      = NUMGADWIDTH;
	ng.ng_GadgetText = "_KiloB DL:";
	ng.ng_GadgetID   = KBDL;

	kbdl=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	kbdl->UserData=(APTR)INTEGER_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3;
	ng.ng_GadgetText = "Ki_loB UL:";
	ng.ng_GadgetID   = KBUL;

	kbul=gad=CreateGadget(INTEGER_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	kbul->UserData=(APTR)INTEGER_KIND;

/* Cycle.... */

	ng.ng_LeftEdge	 = LEFT_COL2-LABEL;
	ng.ng_TopEdge	 = TOP_ROW7;
	ng.ng_Width      = CYCLEGADWIDTH;
	ng.ng_GadgetText = NULL;
	ng.ng_GadgetID	 = EDITOR;

	editor=gad=CreateGadget(CYCLE_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTCY_Labels,	&editorlist,
		TAG_END, TAG_END);
	editor->UserData=(APTR)CYCLE_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3-LABEL;
	ng.ng_GadgetID	 = HELP;

	help=gad=CreateGadget(CYCLE_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTCY_Labels,	&helplist,
		TAG_END, TAG_END);
	help->UserData=(APTR)CYCLE_KIND;

	ng.ng_LeftEdge	 = LEFT_COL2-LABEL;
	ng.ng_TopEdge	 = TOP_ROW6;
	ng.ng_GadgetID	 = COMMAND;

	command=gad=CreateGadget(CYCLE_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTCY_Labels,	&commandlist,
		TAG_END, TAG_END);
	command->UserData=(APTR)CYCLE_KIND;

	ng.ng_LeftEdge	 = LEFT_COL3-LABEL;
	ng.ng_GadgetID	 = GRAPHICS;

	graphics=gad=CreateGadget(CYCLE_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTCY_Labels,	&graphicslist,
		TAG_END, TAG_END);
	graphics->UserData=(APTR)CYCLE_KIND;

/* Check */

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL;
	ng.ng_TopEdge		= TOP_ROW8;
	ng.ng_GadgetID		= FLAGS0;
	ng.ng_Width			= CHECKGADWIDTH;

	flags[0]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[0]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS1;

	flags[1]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[1]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+2*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS2;

	flags[2]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[2]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+3*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS3;

	flags[3]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[3]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+4*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS4;

	flags[4]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[4]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+5*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS5;

	flags[5]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[5]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+6*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS6;

	flags[6]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[6]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge		= LEFT_COL2-NUMLABEL+7*(CHECKGADWIDTH+2);
	ng.ng_GadgetID		= FLAGS7;

	flags[7]=gad=CreateGadget(CHECKBOX_KIND, gad, &ng,
		GT_Underscore,	'_',
		TAG_END, TAG_END);
	flags[7]->UserData=(APTR)CHECKBOX_KIND;

	ng.ng_LeftEdge   = LEFT_COL1-LABEL;
	ng.ng_TopEdge    = TOP_ROW12;
	ng.ng_Width      = TEXTGADWIDTH+LABEL;
	ng.ng_Flags		 = PLACETEXT_IN;
	ng.ng_GadgetText = NULL;
	ng.ng_GadgetID   = TEXT;

	text=gad=CreateGadget(TEXT_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTTX_Border,	TRUE,
		TAG_END, TAG_END);
	text->UserData=(APTR)TEXT_KIND;

	ng.ng_LeftEdge   = LEFT_COL2-LABEL;
	ng.ng_TopEdge    = TOP_ROW12;
	ng.ng_Width      = 2*CYCLEGADWIDTH;
	ng.ng_Flags		 = PLACETEXT_IN;
	ng.ng_GadgetText = "Call INav BBS @ +45 31354775";
	ng.ng_GadgetID   = TEXT;

	add=gad=CreateGadget(TEXT_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTTX_Border,	TRUE,
		TAG_END, TAG_END);
	add->UserData=(APTR)TEXT_KIND;


/*	ng.ng_LeftEdge   = LEFT_COL1;
	ng.ng_TopEdge    = TOP_ROW1;
	ng.ng_Width      = TEXTGADWIDTH;
	ng.ng_Height     = GADHEIGHT;
	ng.ng_GadgetText = "_Name:";
	ng.ng_TextAttr   = screen->Font;
	ng.ng_GadgetID   = NAME;
	ng.ng_Flags		 = PLACETEXT_LEFT;
	ng.ng_VisualInfo = vi;
	ng.ng_UserData	 = NULL;

	name=gad=CreateGadget(STRING_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTST_String,	NULL,
		GTST_MaxChars,	128,
		TAG_END, TAG_END);
	name->UserData=(APTR)STRING_KIND;
*/

	if( window=OpenWindowTags(NULL, 
		WA_PubScreen,	screen,
		WA_Flags,	WFLG_DRAGBAR		| WFLG_DEPTHGADGET |
					WFLG_CLOSEGADGET 	| WFLG_SMART_REFRESH |
					WFLG_ACTIVATE, 
		WA_IDCMP,	IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_RAWKEY |
					IDCMP_REFRESHWINDOW | IDCMP_GADGETUP,
		WA_Width,		LEFT_COL3+NUMGADWIDTH+screen->WBorRight+1,
		WA_Height,		TOP_ROW11+2*GADHEIGHT+screen->WBorBottom+2,
		WA_Zoom,		zoom,
		WA_Gadgets,		glist,
		WA_ScreenTitle,	ScreenTitle,
		WA_Title,		Title,
		TAG_END, TAG_END) )
	{
		GT_RefreshWindow(window, NULL);
		return(window);
	}
}

void shutdown()
{

	if( window ) CloseWindow(window); 
	if( glist ) FreeGadgets(glist);
	if( vi ) FreeVisualInfo(vi);
}


UserType *readuser(struct Node *node, char *dirname)
{
	FILE *fh;
	UserType *user=NULL;
	char filename[129];

	setwintitle(Title);

	if( !node )
		return(NULL);

	sprintf(filename, "%s%s", dirname, node->ln_Name);

	if( fh=fopen(filename, "r") )
	{
		if( user=(UserType *)calloc(sizeof(UserType), 1) )
		{
			user->Name			=strdup(node->ln_Name);
			user->Street		=getstring(fh);		
			user->City			=getstring(fh);		
			user->Province		=getstring(fh);		
			user->Phone			=getstring(fh);		
			user->Computer		=getstring(fh);		
			user->Birthdate		=getstring(fh);		
			user->Postalcode	=getstring(fh);		
			user->Handle		=getstring(fh);		
			user->Lastcall		=getstring(fh);		
			user->Password		=getstring(fh);		
			user->Kbdl			=getint(fh);
			user->Filesdl		=getint(fh);
			user->Kbul			=getint(fh);
			user->Filesul		=getint(fh);
			user->Messages		=getint(fh);
			user->Calls			=getint(fh);
			user->Screenlength	=getint(fh);
			user->Fileratio		=getint(fh);
			user->Timelimit		=getint(fh);
			user->Unusedtime	=getint(fh);
			user->Commandmode	=getint(fh);
			user->Graphicsmode	=getint(fh);
			user->Editor		=getint(fh);
			user->Accesslevel	=getint(fh);
			user->Netmailcredit	=getint(fh);
			user->Flags			=getflags(fh);		
			user->Help			=getint(fh);
			user->Usernumber	=getint(fh);
			
			displayuser(user);
		}
		fclose(fh);
	}

	return(user);
}

#define STRINGBUFSIZ 256

char *getstring(FILE *fh)
{
	char *string, buffer[STRINGBUFSIZ];
	int len, i;

	string=fgets(buffer, STRINGBUFSIZ, fh);

	string[strlen(string)-1]='\0';

	len=strlen(string);

	for(i=0; i<len, *string==' ' || *string=='\t' ; i++)
	{
		string++;
	}

	return(strdup(string));
}

int getint(FILE *fh)
{
	int ret;

	ret=atoi(getstring(fh));

	return(ret);
}

FlagsType *getflags(FILE *fh)
{
	int packed;
	FlagsType *flags;

	packed=getint(fh);
	flags=unpackflags(packed);

	return(flags);
}

FlagsType *unpackflags(int packed)
{
	/* Saves as:  1*1 + 0*2 + 0*4 + 1*8 + 1*16 + 0*32 + 1*64 + 0*128 = 89 */

	FlagsType *flags=NULL;
	int i;
	
	if( flags=(FlagsType *)calloc(sizeof(FlagsType), 1) )
	{
		for(i=0; i<8; i++)
		{
			if( packed & (1<<i) )	flags->flag[i]=TRUE;
		}
	}
	return(flags);
}

int packflags(FlagsType *flags)
{
	int packed=NULL;
	int i;
	
	for(i=0; i<8; i++)
	{
		if( flags->flag[i] == TRUE ) packed |= (1<<i);
	}

	return(packed);
}

#define NUM_STRINGS 11

void displayuser(UserType *user)
{
	char *string[NUM_STRINGS];
	struct Gadget *strgad[NUM_STRINGS];
	int nums[12];
	struct Gadget *numgad[12];
	int cycnums[4];
	struct Gadget *cycgad[4];
	static char textstring[81];
	
	int i;

	sprintf(textstring, "Usernumber: %4.4ld/%ld", user->Usernumber, allusers);
	
	GT_SetGadgetAttrs(text, window, NULL,
		GTTX_Text, textstring,
		TAG_END, TAG_END);

	string[0]=user->Name;
	string[1]=user->Handle;
	string[2]=user->Phone;
	string[3]=user->Street;
	string[4]=user->Computer;
	string[5]=user->Postalcode;
	string[6]=user->City;
	string[7]=user->Province;
	string[8]=user->Password;
	string[9]=user->Birthdate;
	string[10]=user->Lastcall;

	strgad[0]=name;
	strgad[1]=handle;
	strgad[2]=phone;
	strgad[3]=street;
	strgad[4]=computer;
	strgad[5]=postalcode;
	strgad[6]=city;
	strgad[7]=province;
	strgad[8]=password;
	strgad[9]=birth;
	strgad[10]=lastcall;

	for(i=0; i<NUM_STRINGS; i++)
	{
		GT_SetGadgetAttrs(strgad[i], window, NULL,
			GTST_String, string[i],
			TAG_END, TAG_END);
	}

	nums[0]=user->Accesslevel;
	nums[1]=user->Fileratio;
	nums[2]=user->Timelimit;
	nums[3]=user->Unusedtime;
	nums[4]=user->Kbdl;
	nums[5]=user->Kbul;
	nums[6]=user->Filesdl;
	nums[7]=user->Filesul;
	nums[8]=user->Messages;
	nums[9]=user->Calls;
	nums[10]=user->Screenlength;
	nums[11]=user->Netmailcredit;

	numgad[0]=accesslevel;
	numgad[1]=ratio;
	numgad[2]=timelimit;
	numgad[3]=unused;
	numgad[4]=kbdl;
	numgad[5]=kbul;
	numgad[6]=fidl;
	numgad[7]=fiul;
	numgad[8]=msgs;
	numgad[9]=calls;
	numgad[10]=scrlength;
	numgad[11]=netmail;
	
	for(i=0; i<12; i++)
	{
		GT_SetGadgetAttrs(numgad[i], window, NULL,
			GTIN_Number, nums[i],
			TAG_END, TAG_END);
	}

	cycnums[0]=user->Commandmode;
	cycnums[1]=user->Graphicsmode;
	cycnums[2]=user->Editor;
	cycnums[3]=user->Help;

	cycgad[0]=command;
	cycgad[1]=graphics;
	cycgad[2]=editor;
	cycgad[3]=help;

	for(i=0; i<4; i++)
	{
		GT_SetGadgetAttrs(cycgad[i], window, NULL,
			GTCY_Active, cycnums[i],
			TAG_END, TAG_END);
	}

	for(i=0; i<8; i++)
	{
		GT_SetGadgetAttrs(flags[i], window, NULL,
			GTCB_Checked, user->Flags->flag[i],
			TAG_END, TAG_END);
	}

}

void writeuser(UserType *user, char *dirname)
{
	FILE *fh;
	char filename[129];

	if( !user )
		return;

	sprintf(filename, "%s%s", dirname, user->Name);

	if( fh=fopen(filename, "w") )
	{
		fprintf(fh, "%s\n", user->Street);
		fprintf(fh, "%s\n", user->City);
		fprintf(fh, "%s\n", user->Province);
		fprintf(fh, "%s\n", user->Phone);
		fprintf(fh, "%s\n", user->Computer);
		fprintf(fh, "%s\n", user->Birthdate);
		fprintf(fh, "%s\n", user->Postalcode);
		fprintf(fh, "%s\n", user->Handle);
		fprintf(fh, "%s\n", user->Lastcall);
		fprintf(fh, "%s\n", user->Password);
		fprintf(fh, " %d \n", user->Kbdl);
		fprintf(fh, " %d \n", user->Filesdl);
		fprintf(fh, " %d \n", user->Kbul);
		fprintf(fh, " %d \n", user->Filesul);
		fprintf(fh, " %d \n", user->Messages);
		fprintf(fh, " %d \n", user->Calls);
		fprintf(fh, " %d \n", user->Screenlength);
		fprintf(fh, " %d \n", user->Fileratio);
		fprintf(fh, " %d \n", user->Timelimit);
		fprintf(fh, " %d \n", user->Unusedtime);
		fprintf(fh, " %d \n", user->Commandmode);
		fprintf(fh, " %d \n", user->Graphicsmode);
		fprintf(fh, " %d \n", user->Editor);
		fprintf(fh, " %d \n", user->Accesslevel);
		fprintf(fh, " %d \n", user->Netmailcredit);
		fprintf(fh, " %d \n", packflags(user->Flags));
		fprintf(fh, " %d \n", user->Help);
		fprintf(fh, " %d \n", user->Usernumber);
		fprintf(fh, "\n\n\n\n\n\n\n\n\n\n");	/* Padding... */			

		fclose(fh);
		setwintitle("Userdata saved.");
	}
}

void setwintitle(char *title)
{
	SetWindowTitles(window, title, (UBYTE *)~0);
}

void showusers(struct List *userlist, char *dirname, char *systemdirname)
{
	BOOL done = FALSE;
	struct IntuiMessage *im;
	ULONG class;
	UWORD code;
	ULONG signal;
	ULONG sec, mic;

/*	ULONG lastsec, lastmic;
	UWORD lastcode;
*/
	int actual;

	struct Gadget *gad;
	UserType *user;
	struct Node *node, *newnode;
	char *newname;
	char *delargarray[3] = { NULL, NULL, NULL};

/*	struct EasyStruct reallydel =
	{
		sizeof(struct EasyStruct),
		0,
		"NiceEd request",
		"Really delete %s?",
		"Yes|No",
	};
*/	
	window=getwindow();

	node=userlist->lh_Head;

	user=readuser(node, dirname);
	
	while( !done )
	{
		signal = Wait(1L << window->UserPort->mp_SigBit | SIGBREAKF_CTRL_C);

		if( signal & SIGBREAKF_CTRL_C)
		{
			done=TRUE;
			continue;
		}

		while( im=GT_GetIMsg(window->UserPort) )
		{
			class=im->Class;
			code =im->Code;
			gad  =(struct Gadget *)(im->IAddress);
			sec  =im->Seconds;
			mic  =im->Micros;
	
			GT_ReplyIMsg(im);
			
			switch( class )
			{
				case IDCMP_CLOSEWINDOW:
					done=TRUE;
				break;
					
				case IDCMP_REFRESHWINDOW:
					GT_BeginRefresh(window);
					GT_EndRefresh(window, TRUE);
				break;
				
				case GADGETUP:
					switch( gad->GadgetID )
					{
						case NAME:
							ActivateGadget(handle, window, NULL);
						break;
						case HANDLE:
							ActivateGadget(phone, window, NULL);
						break;
						case PHONE:
							ActivateGadget(street, window, NULL);
						break;
						case STREET:
							ActivateGadget(postalcode, window, NULL);
						break;
						case POSTALCODE:
							ActivateGadget(city, window, NULL);
						break;
						case CITY:
							ActivateGadget(province, window, NULL);
						break;
						case PROVINCE:
							ActivateGadget(computer, window, NULL);
						break;
						case COMPUTER:
							ActivateGadget(password, window, NULL);
						break;
						case PASSWORD:
							ActivateGadget(accesslevel, window, NULL);
						break;
						case ACCESSLEVEL:
							ActivateGadget(ratio, window, NULL);
						break;
						case RATIO:
							ActivateGadget(timelimit, window, NULL);
						break;
						case TIMELIMIT:
							ActivateGadget(unused, window, NULL);
						break;
						case UNUSED:
							ActivateGadget(kbdl, window, NULL);
						break;
						case KBDL:
							ActivateGadget(kbul, window, NULL);
						break;
						case KBUL:
							ActivateGadget(fidl, window, NULL);
						break;
						case FIDL:
							ActivateGadget(fiul, window, NULL);
						break;
						case FIUL:
							ActivateGadget(birth, window, NULL);
						break;
						case BIRTH:
							ActivateGadget(lastcall, window, NULL);
						break;
						case LASTCALL:
							ActivateGadget(msgs, window, NULL);
						break;
						case MSGS:
							ActivateGadget(calls, window, NULL);
						break;
						case CALLS:
							ActivateGadget(scrlength, window, NULL);
						break;
						case SCRLENGTH:
							ActivateGadget(netmail, window, NULL);
						break;
						case NETMAIL:
							ActivateGadget(name, window, NULL);
						break;

						case COMMAND:
							user->Commandmode=1-user->Commandmode;
							GT_SetGadgetAttrs(command, window, NULL, GTCY_Active, user->Commandmode);
						break;							
						case GRAPHICS:
							user->Graphicsmode=1-user->Graphicsmode;
							GT_SetGadgetAttrs(graphics, window, NULL, GTCY_Active, user->Graphicsmode);
						break;							
						case EDITOR:
							user->Editor=1-user->Editor;
							GT_SetGadgetAttrs(editor, window, NULL, GTCY_Active, user->Editor);
						break;							
						case HELP:
							user->Help=1-user->Help;
							GT_SetGadgetAttrs(help, window, NULL, GTCY_Active, user->Help);
						break;							

						/* flags[x]->flags = 0x86 (checked) 0x06 (not checked) */
						case FLAGS0:
						case FLAGS1:
						case FLAGS2:
						case FLAGS3:
						case FLAGS4:
						case FLAGS5:
						case FLAGS6:
						case FLAGS7:
							actual=gad->GadgetID-FLAGS0;	/* 0 to 7 */
							user->Flags->flag[actual]=1-user->Flags->flag[actual];
							GT_SetGadgetAttrs(flags[actual], window, NULL, GTCB_Checked, user->Flags->flag[actual]);
						break;

						case CHOOSE:	/* Open window with SCROLL_LIST */
							if( node=choosewin(userlist) )
							{
								user=readuser(node, dirname);
							}
						break;
						
						case SAVE:
							setwintitle("Saving user...");

							user->Name=strdup(((struct StringInfo *)name->SpecialInfo)->Buffer);
							user->Handle=strdup(((struct StringInfo *)handle->SpecialInfo)->Buffer);
							user->Phone=strdup(((struct StringInfo *)phone->SpecialInfo)->Buffer);
							user->Street=strdup(((struct StringInfo *)street->SpecialInfo)->Buffer);
							user->Postalcode=strdup(((struct StringInfo *)postalcode->SpecialInfo)->Buffer);
							user->City=strdup(((struct StringInfo *)city->SpecialInfo)->Buffer);
							user->Province=strdup(((struct StringInfo *)province->SpecialInfo)->Buffer);
							user->Computer=strdup(((struct StringInfo *)computer->SpecialInfo)->Buffer);
							user->Password=strdup(((struct StringInfo *)password->SpecialInfo)->Buffer);
							user->Birthdate=strdup(((struct StringInfo *)birth->SpecialInfo)->Buffer);
							user->Lastcall=strdup(((struct StringInfo *)lastcall->SpecialInfo)->Buffer);

							user->Accesslevel=((struct StringInfo *)accesslevel->SpecialInfo)->LongInt;
							user->Fileratio=((struct StringInfo *)ratio->SpecialInfo)->LongInt;
							user->Timelimit=((struct StringInfo *)timelimit->SpecialInfo)->LongInt;
							user->Unusedtime=((struct StringInfo *)unused->SpecialInfo)->LongInt;
							user->Kbdl=((struct StringInfo *)kbdl->SpecialInfo)->LongInt;
							user->Kbul=((struct StringInfo *)kbul->SpecialInfo)->LongInt;
							user->Filesdl=((struct StringInfo *)fidl->SpecialInfo)->LongInt;
							user->Filesul=((struct StringInfo *)fiul->SpecialInfo)->LongInt;
							user->Messages=((struct StringInfo *)msgs->SpecialInfo)->LongInt;
							user->Calls=((struct StringInfo *)calls->SpecialInfo)->LongInt;
							user->Screenlength=((struct StringInfo *)scrlength->SpecialInfo)->LongInt;
							user->Netmailcredit=((struct StringInfo *)netmail->SpecialInfo)->LongInt;

							writeuser(user, dirname);
						break;
						
						case NEXT:
							if( (node->ln_Succ)->ln_Succ != NULL )
							{
								node=node->ln_Succ;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;
						case PREV:
							if( (node->ln_Pred)->ln_Pred != NULL )
							{
								node=node->ln_Pred;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;

						case NEW:	/* Handle newusers struct Gadget */
							if( new->Flags & GFLG_DISABLED )
							{
								break;
							}
							else
							{
								if( !(newname=getnewname(systemdirname)) )
								{
									DisplayBeep(screen);
									GT_SetGadgetAttrs(new, window, NULL, GA_Disabled, TRUE);
								}
								else
								{
									if( newnode=findnode(userlist, newname) )
									{
										node=newnode;
										user=readuser(node, dirname);
									}
								}
							}
						break;
						
						case NORMAL:
							user->Accesslevel=10;
							user->Flags->flag[0]=0;
							user->Flags->flag[1]=0;
							user->Flags->flag[2]=0;
							user->Flags->flag[3]=0;
							user->Flags->flag[4]=0;
							user->Flags->flag[5]=0;
							user->Flags->flag[6]=0;
							user->Flags->flag[7]=0;
							GT_SetGadgetAttrs(accesslevel, window, NULL, GTIN_Number, user->Accesslevel);
							for(actual=0; actual<8; actual++)
								GT_SetGadgetAttrs(flags[actual], window, NULL, GTCB_Checked, user->Flags->flag[actual]);
						break;

					}
				break;

				case VANILLAKEY:
					switch( ToLower((long)code) )
					{
						case 'n':
							ActivateGadget(name, window, NULL);
						break;
						case 'h':
							ActivateGadget(handle, window, NULL);
						break;
						case 'p':
							ActivateGadget(phone, window, NULL);
						break;
						case 't':
							ActivateGadget(street, window, NULL);
						break;
						case 'o':
							ActivateGadget(postalcode, window, NULL);
						break;
						case 'r':
							ActivateGadget(province, window, NULL);
						break;
						case 'c':
							ActivateGadget(computer, window, NULL);
						break;
						case 'a':
							ActivateGadget(password, window, NULL);
						break;
						case 'e':
							ActivateGadget(accesslevel, window, NULL);
						break;
						case 'i':
							ActivateGadget(ratio, window, NULL);
						break;
						case 'm':
							ActivateGadget(timelimit, window, NULL);
						break;
						case 'u':
							ActivateGadget(unused, window, NULL);
						break;
						case 'k':
							ActivateGadget(kbdl, window, NULL);
						break;
						case 'l':
							ActivateGadget(kbul, window, NULL);
						break;
						case 'f':
							ActivateGadget(fidl, window, NULL);
						break;

/*						case '':
							ActivateGadget(fiul, window, NULL);
						break;
*/
						case 'b':
							ActivateGadget(birth, window, NULL);
						break;
/*						case '':
							ActivateGadget(lastcall, window, NULL);
						break;
*/
						case 'g':
							ActivateGadget(scrlength, window, NULL);
						break;

						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
							actual=code-(int)'1';
							user->Flags->flag[actual]=1-user->Flags->flag[actual];
							GT_SetGadgetAttrs(flags[actual], window, NULL, GTCB_Checked, user->Flags->flag[actual]);
						break;


						case 'z':
							ZipWindow(window);
						break;

						case 's':
							writeuser(user, dirname);
						break;

						case 'q':
						case 'x':
							if( really(" Do you want to quit? ", " _Yes | _No ", "NiceEd termination...", NULL) )
							{
								done=TRUE;
							}
						break;
						
						case '<':
							if( (node->ln_Pred)->ln_Pred != NULL )
							{
								node=node->ln_Pred;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;						

						case '>':
							if( (node->ln_Succ)->ln_Succ != NULL )
							{
								node=node->ln_Succ;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;
						case 'd':
						case 0x7f: /* Del */
							delargarray[0]=node->ln_Name;
							delargarray[1]=user->Handle;

							if( really(" Really delete %s (%s)? ", " _Yes | _No ", "NiceEd - deletion confirmation request", delargarray) )
							{
								if( (node->ln_Succ)->ln_Succ != NULL )
								{
									newnode=node->ln_Succ;
								}
								else
								{
									newnode=node->ln_Pred;
								}

								if( !deleteuser(node, user, userlist, dirname) )
								{
									really(" %s (%s) couldn't be deleted! ", " Don't know why ", "NiceEd error information", delargarray);
								}
								else
								{
									node=newnode;
									user=readuser(node, dirname);
								}
							}
						
/*							if( EasyRequestArgs(window, &reallydel, NULL, node->ln_Name) )
							{
								printf("Deleted %s\n", node->ln_Name);
							}
*/							
						break;
					}
				break;

				case RAWKEY:
					switch( code )
					{
						case 0x4f:	/* <- */
							if( (node->ln_Pred)->ln_Pred != NULL )
							{
								node=node->ln_Pred;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;						

						case 0x4e:	/* -> */
							if( (node->ln_Succ)->ln_Succ != NULL )
							{
								node=node->ln_Succ;
								user=readuser(node, dirname);
							}
							else
								DisplayBeep(screen);
						break;

						case 0x5f:	/* Help */
							really("\nNiceEd\nThe Real TransAmiga Dooms-days machine\nVersion 0.01\nPublic Domain 1993 by\nAdam Sjoegren (2:230/149@fidonet)\nNo Rights Reserved what so ever\n\nSo there!\n\nCall my BBS:\nInterstellar Navigation * +45 31354775\n24 hours * 16.8k USR Dual\n\n", " Wauw, gee, thanks for the PD! ", "NiceEd 'about' information screen", NULL);
						break;
						
/*						default:
							printf("raw: 0x%x\n", code);
						break;
*/
					}
				break;
			}

		}
	}

	shutdown();
}

#define CWIN_WIDTH	220
#define CWIN_CHOOSE	0

struct Node *choosewin(struct List *userlist)
{
	struct Window *cwin;
	struct Gadget *gadcwinlist, *gad;
	struct NewGadget ng;

	BOOL done = FALSE;
	struct IntuiMessage *im;
	ULONG class;
	UWORD code;
	ULONG signal;
	ULONG sec, mic;

	ULONG lastsec, lastmic;
	UWORD lastcode;

	struct Node *newnode=NULL;

	gadcwinlist=NULL;
	gad=CreateContext(&gadcwinlist);

	ng.ng_LeftEdge	 = screen->WBorLeft+1;
	ng.ng_TopEdge	 = screen->WBorTop+screen->BarHeight;
	ng.ng_Width      = CWIN_WIDTH - (screen->WBorLeft+1+screen->WBorRight+1);
	ng.ng_Height     = screen->Height-(2*ng.ng_TopEdge);
	ng.ng_GadgetText = NULL;
	ng.ng_TextAttr   = screen->Font;
	ng.ng_GadgetID   = CWIN_CHOOSE;
	ng.ng_Flags		 = NULL;
	ng.ng_VisualInfo = vi;
	ng.ng_UserData	 = NULL;

	gad=CreateGadget(LISTVIEW_KIND, gad, &ng,
		GT_Underscore,	'_',
		GTLV_Labels,	userlist,
		TAG_END);
	gad->UserData=(APTR)LISTVIEW_KIND;

	if( cwin=OpenWindowTags(NULL, 
		WA_PubScreen,	screen,
		WA_Flags,	WFLG_DRAGBAR		| WFLG_DEPTHGADGET |
					WFLG_CLOSEGADGET 	| WFLG_SMART_REFRESH |
					WFLG_ACTIVATE, 
		WA_IDCMP,	IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY |
					IDCMP_REFRESHWINDOW | LISTVIEWIDCMP,
		WA_Width,		CWIN_WIDTH,
		WA_Height, 		screen->Height-screen->BarHeight-1,
		WA_Gadgets,		gadcwinlist,
		WA_ScreenTitle,	ScreenTitle,
		WA_Title,		"Choose user to edit",
		TAG_END, TAG_END) )
	{
		GT_RefreshWindow(cwin, NULL);
	}

	lastsec=lastmic=0L;
	lastcode=0;

	while( !done )
	{
		signal = Wait(1L << cwin->UserPort->mp_SigBit | SIGBREAKF_CTRL_C);

		if( signal & SIGBREAKF_CTRL_C)
		{
			done=TRUE;
			continue;
		}

		while( im=GT_GetIMsg(cwin->UserPort) )
		{
			class=im->Class;
			code =im->Code;
			gad  =(struct Gadget *)(im->IAddress);
			sec  =im->Seconds;
			mic  =im->Micros;
	
			GT_ReplyIMsg(im);
			
			switch( class )
			{
				case IDCMP_CLOSEWINDOW:
					done=TRUE;
				break;
					
				case IDCMP_REFRESHWINDOW:
					GT_BeginRefresh(cwin);
					GT_EndRefresh(cwin, TRUE);
				break;
				
				case GADGETUP:
					switch( gad->GadgetID )
					{
						/* Code = nodenumber */
						case CWIN_CHOOSE:
							newnode=getnodenumber(userlist, code);
							if( code==lastcode )
							{
								if( DoubleClick(lastsec, lastmic, sec, mic) )
									done=TRUE;
							}
							lastsec =sec;
							lastmic =mic;
							lastcode=code;
						break;
					}
				break;
			}
		}
	}

	if( cwin ) CloseWindow(cwin); 
	if( gadcwinlist ) FreeGadgets(gadcwinlist);

	return(newnode);
}

struct Node *getnodenumber(struct List *list, UWORD number)
{
	int i;
	struct Node *node;

	node=list->lh_Head;

	for(i=0; i<number; i++)
	{
		node=node->ln_Succ;
	}
	
	return(node);
}

#define NEWTEMPNAME "t:NiceEd.tmp"

char *getnewname(char *systemdirname)
{
	FILE *fh, *out;
	char filename[128];
	char buf[128];
	char *line, *name=NULL;
	BOOL ok=FALSE;
	int size;

	sprintf(filename, "%s%s", systemdirname, "NewUsers.BBS");

	if( fh=fopen(filename, "r") )
	{
		if( line=fgets(buf, 127, fh) )
			line[strlen(line)-1]=NULL;
		
		name=strdup(line);
		
		if( out=fopen(NEWTEMPNAME, "w") )
		{
			ok=TRUE;
			while( !feof(fh) )
			{
				if( line=fgets(buf, 127, fh) )
					fputs(line, out);
				else
					break;
			}
			fclose(out);
		}
		fclose(fh);
	}

	/* Copy back to NewUsers.bbs */
	if( fh=fopen(NEWTEMPNAME, "r") )
	{
		fseek(fh, 0, SEEK_END);
		size=ftell(fh);
		fseek(fh, 0, SEEK_SET);
		
		if( size>3 )
		{
			if( out=fopen(filename, "w") )
			{
				while( !feof(fh) )
				{
					if( line=fgets(buf, 127, fh) )
						fputs(line, out);
					else
						break;
				}
				fclose(out);
			}
		}
		else
		{
			DeleteFile(filename);
		}
		fclose(fh);
	}

	DeleteFile(NEWTEMPNAME);
	return(name);
}

struct Node *findnode(struct List *list, char *findtext)
{
	struct Node *node, *retnode=NULL;
	int len;

	len = strlen(findtext);
	
	if( node=list->lh_Head )
	{
		while( node )
		{
			if( strnicmp(node->ln_Name, findtext, len)==0 )
			{
				retnode=node;
				break;
			}
			
			node=node->ln_Succ;		
		}
	}

	return(retnode);
}

unsigned long really(char *question, char *gadgets, char *title, APTR argarray)
{
	return( rtEZRequestTags(question, gadgets, NULL, argarray,
				RT_Window, window,
				RT_ReqPos, REQPOS_CENTERWIN,
				RT_WaitPointer, TRUE,
				RT_LockWindow, TRUE,
				RT_Underscore, '_',
				RTEZ_ReqTitle, title,
				RTEZ_Flags, EZREQF_CENTERTEXT,
				TAG_END, TAG_END) );	

}

BOOL deleteuser(struct Node *node, UserType *user, struct List *userlist, char *dirname)
{
	char buf[256];
	BOOL ok=TRUE;
	
	/* Delete userfile */
	sprintf(buf, "%s%s", dirname, node->ln_Name);
	if( !(DeleteFile(buf)) )
		ok=FALSE;

	/* Delete handle-file too */
	if( strlen(user->Handle) > 0 )
	{
		sprintf(buf, "%s@%s", dirname, user->Handle);

		if( !(DeleteFile(buf)) )
			ok=FALSE;
	}

	/* Remove from list */
	Remove(node);

	return(ok);
}

/*
** Thanks to Doug Walker, SAS Institute, for this hint from InterNet:
*/

int rawsprintf(char *buffer, char *ctl, ...)
{
   va_list args;

   va_start(args, ctl);

   /*********************************************************/
   /* NOTE: The string below is actually CODE that copies a */
   /*       value from D0 to A3 and increments A3:          */
   /*                                                       */
   /*          move.b d0,(a3)+                              */
   /*          rts                                          */
   /*                                                       */
   /*       It is essentially the callback routine needed   */
   /*       by RawDoFmt.                                    */
   /*********************************************************/

   RawDoFmt(ctl, args, (void (*))"\x16\xc0\x4e\x75", buffer);

   va_end(args);

   return((int)strlen(buffer));
}
