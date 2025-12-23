#include <stdio.h>
#include <stdlib.h>
#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>

#include "SysopActivity.h"

struct Screen         *Scr = NULL;
UBYTE                 *PubScreenName = "Workbench";
APTR                   VisualInfo = NULL;
struct Window         *SysopWnd = NULL;
struct Gadget         *SysopGList = NULL;
struct IntuiMessage    SysopMsg;
struct Gadget         *SysopGadgets[1];
UWORD                  SysopLeft = 470;
UWORD                  SysopTop = 15;
UWORD                  SysopWidth = 195;
UWORD                  SysopHeight = 89;
UBYTE                 *SysopWdt = (UBYTE *)"Written by Ari Tsironis";

extern struct MinList Activity0List;

struct Node Activity0Nodes[] = {
	&Activity0Nodes[1], ( struct Node * )&Activity0List.mlh_Head, 0, 0, "SYSOP_AVAIL",
	&Activity0Nodes[2], &Activity0Nodes[0], 0, 0, "SYSOP_BUSY",
	&Activity0Nodes[3], &Activity0Nodes[1], 0, 0, "SYSOP_PISSED",
	&Activity0Nodes[4], &Activity0Nodes[2], 0, 0, "SYSOP_WATCHING",
	&Activity0Nodes[5], &Activity0Nodes[3], 0, 0, "SYSOP_ASLEEP",
	( struct Node * )&Activity0List.mlh_Tail, &Activity0Nodes[4], 0, 0, "SYSOP_NOT_AVAIL" };

struct MinList Activity0List = 
{
	( struct MinNode * )&Activity0Nodes[0], 
  ( struct MinNode * )NULL, 
  ( struct MinNode * )&Activity0Nodes[5] 
};

struct TextAttr XEN9 = 
{
	( STRPTR )"XEN.font",
  9,
  0x00,
  0x42
};

struct IntuiText text =
{
  2, 3,
  JAM2,
  0, 0,
  &XEN9,
  NULL,
  NULL
};

UWORD SysopGTypes[] = {
	LISTVIEW_KIND
};

struct NewGadget SysopNGad[] = {
	15, 15, 156, 64, (UBYTE *)"MAXS Sysop Activity Controller", NULL, GD_Activity, PLACETEXT_ABOVE, NULL, (APTR)ActivityClicked
};

ULONG SysopGTags[] = {
	(GTLV_Labels), (ULONG)&Activity0List, (TAG_DONE)
};



main()
{
  int done=0;
  ULONG signalmask, winsignal, signals;
  
  
  if(SetupScreen())
    {
      CloseDownScreen();
      exit(10);
    }
    
  if(OpenSysopWindow())
    {
      CloseDownScreen();
      CloseSysopWindow();
      exit(10);
    }
  
  winsignal = 1L << SysopWnd->UserPort->mp_SigBit;  /* window IDCMP */
  signalmask = winsignal;   /* we are only waiting on IDCMP events */

 
  PrintSysop(SysopWnd->RPort);
  
  while(done==0)
    {
      signals = Wait(signalmask);
  
      if(signals & winsignal)
        done=HandleSysopIDCMP();
    }
      
  
  CloseSysopWindow();
  CloseDownScreen();
}


void ActivityClicked(int button)
{
  FILE *fp;
  
  
  if((fp=fopen("DOORS:SysopActivity/SysopActivity.config","w"))==NULL)
    {
      DisplayBeep(Scr);
      return;
    }
    
  switch(button)
    {
      case 0:fprintf(fp,"BBS:Text/Sysop_Avail.Text");
             break;
             
      case 1:fprintf(fp,"BBS:Text/Sysop_Busy.Text");
             break;
             
      case 2:fprintf(fp,"BBS:Text/Sysop_Pissed.Text");
             break;
             
      case 3:fprintf(fp,"BBS:Text/Sysop_Watching.Text");
             break;
             
      case 4:fprintf(fp,"BBS:Text/Sysop_Asleep.Text");
             break;
             
      case 5:fprintf(fp,"BBS:Text/Sysop_Not_Avail.Text");
             break;
             
      default: break;
      
    }
    
  fclose(fp);

  PrintSysop(SysopWnd->RPort);  
}


void PrintSysop(struct RastPort *rp)
{
  LONG length;
  char str[255];
  FILE *fp;
  
  
  if((fp=fopen("DOORS:SysopActivity/SysopActivity.config","r"))==NULL)
    {
      DisplayBeep(Scr);
      return;
    }

  
  strcpy(str,"                             ");
  text.FrontPen=1;
  text.BackPen=0;
  text.IText=str;
  PrintIText(rp,&text,10,90);
  
  fgets(str,80,fp);
  text.FrontPen=2;
  text.BackPen=3;
  text.IText=str;
  length=IntuiTextLength(&text);
  PrintIText(rp,&text,(SysopWnd->Width-length)/2,90);

  fclose(fp);
}



int SetupScreen( void )
{
	if ( ! ( Scr = LockPubScreen( PubScreenName )))
		return( 1L );

	if ( ! ( VisualInfo = GetVisualInfo( Scr, TAG_DONE )))
		return( 2L );

	return( 0L );
}

void CloseDownScreen( void )
{
	if ( VisualInfo ) {
		FreeVisualInfo( VisualInfo );
		VisualInfo = NULL;
	}

	if ( Scr        ) {
		UnlockPubScreen( NULL, Scr );
		Scr = NULL;
	}
}

int HandleSysopIDCMP( void )
{
	struct IntuiMessage	*m;
  int done=0;
  

	while( m = GT_GetIMsg( SysopWnd->UserPort )) {

		CopyMem(( char * )m, ( char * )&SysopMsg, (long)sizeof( struct IntuiMessage ));

		GT_ReplyIMsg( m );

		switch ( SysopMsg.Class ) {

			case	IDCMP_REFRESHWINDOW:
				GT_BeginRefresh( SysopWnd );
				GT_EndRefresh( SysopWnd, TRUE );
				break;

			case	IDCMP_CLOSEWINDOW:
				done = -1;
				break;

			case	IDCMP_GADGETUP:
			case	IDCMP_GADGETDOWN:
				ActivityClicked(SysopMsg.Code);
				break;
		}
	}
	return(done);
}

int OpenSysopWindow( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

	if ( ! ( g = CreateContext( &SysopGList )))
		return( 1L );

	for( lc = 0, tc = 0; lc < Sysop_CNT; lc++ ) {

		CopyMem((char * )&SysopNGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &XEN9;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		SysopGadgets[ lc ] = g = CreateGadgetA((ULONG)SysopGTypes[ lc ], g, &ng, ( struct TagItem * )&SysopGTags[ tc ] );

		while( SysopGTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	if ( ! ( SysopWnd = OpenWindowTags( NULL,
				WA_Left,	SysopLeft,
				WA_Top,		SysopTop,
				WA_Width,	SysopWidth,
				WA_Height,	SysopHeight + offy,
				WA_IDCMP,	LISTVIEWIDCMP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_SMART_REFRESH,
				WA_Gadgets,	SysopGList,
				WA_Title,	SysopWdt,
				WA_ScreenTitle,	"MAXS Sysop Activity Controller by Ari Tsironis",
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( SysopWnd, NULL );

	return( 0L );
}

void CloseSysopWindow( void )
{
	if ( SysopWnd        ) {
		CloseWindow( SysopWnd );
		SysopWnd = NULL;
	}

	if ( SysopGList      ) {
		FreeGadgets( SysopGList );
		SysopGList = NULL;
	}
}

