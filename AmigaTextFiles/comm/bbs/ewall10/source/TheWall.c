/*
 The Wall 1.0 by Celso Martinho in 30/3/93
 To use with Excelsior! BBS Software ONLY!

 Suggestions, help or E! soft trading write to:
 Internet: etcelso@zeus.ua.pt
 FidoNet: Celso Martinho>2:361/9.1
 Post: P.O.BOX 2037, 3800 Aveiro, Portugal
*/

#include "ex-c-doors.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define footer_file "s:TheWall.footer"
#define header_file "s:TheWall.header"
#define data_file "s:TheWall.data"

char file_name[32];
char Wall[12][100];
char Message[60];
char UserName[32];
char DataLine[100];

ULONG *TempPtr;

struct	MsgPort			*reply, *xport;
struct	Common_Data		*cd;
struct	Comm_Message	*cs;

VOID	CloseDown( UBYTE error );
ULONG	*WhatIs( ULONG t );
ULONG	GetLine( TEXT *t, ULONG flags, ULONG maxc );
LONG	SetUpCommand( ULONG cmd );
LONG	PassCommand( struct Comm_Message *comm );
LONG	PrintString( TEXT *string );
LONG	GetCH( VOID );
LONG	Logoff( VOID );
LONG	PrintFile( TEXT *file, UWORD flags );

VOID CloseDown( UBYTE error )
{
	if (SetUpCommand(CMD_CCLOSE_DOWN))	
		PassCommand(cs);

	if (reply)
		DeleteMsgPort(reply);

	exit(error);
}



ULONG *WhatIs( ULONG t )
{
ULONG	x;


	if (SetUpCommand(CMD_WHATIS))
		{
		cs->Pass1 = (ULONG)t;
		x = PassCommand(cs);
		FreeVec(cs);

		return (ULONG *)x;
		}
	return 0l;
}


ULONG GetLine( TEXT *t, ULONG flags, ULONG maxc )
{
ULONG	x;

	if (SetUpCommand(CMD_GETLINE))
		{
		cs->Pass1 = (ULONG)t;
		cs->Pass2 = (ULONG)flags;
		cs->Pass3 = (ULONG)maxc;

		x = PassCommand(cs);
		FreeVec(cs);

		return x;
		}
	return 0;
}


LONG SetUpCommand( ULONG cmd )
{
	if (!(cs = (struct Comm_Message *) AllocVec(sizeof(struct Comm_Message), MEMF_CLEAR|MEMF_PUBLIC)))
		return FALSE;

	cs->Command				 = (ULONG)cmd;
	cs->message.mn_Length    = sizeof(struct Comm_Message);
	cs->message.mn_ReplyPort = reply;

	return TRUE;
}


LONG PassCommand( struct Comm_Message *comm )
{
	PutMsg(xport, (struct Message *)comm);

	WaitPort(reply);
	GetMsg(reply);

	return (LONG)comm->Result1;
}


LONG PrintString( TEXT *string )
{
LONG	x;

	if (SetUpCommand(CMD_PRINT_STRING))
		{
		cs->Pass1 = (ULONG)string;
		x = PassCommand(cs);
		FreeVec(cs);
		return x;
		}
	return 0;
}


LONG PrintFile( TEXT *file, UWORD flags )
{
LONG	x;

	if (SetUpCommand(CMD_PRINT_FILE))
		{
		cs->Pass1 = (ULONG)file;
		cs->Pass2 = (ULONG)flags;
		x = PassCommand(cs);
		FreeVec(cs);
		return x;
		}
	return 0;
}


LONG GetCH( VOID )
{
LONG	x;

	if (SetUpCommand(CMD_GETCH))
		{
		x = PassCommand(cs);
		FreeVec(cs);
		return x;
		}
	return 0;

}


LONG Logoff( VOID )
{
LONG	x;

	if (SetUpCommand(CMD_IS_LOGOFF))
		{
		x = PassCommand(cs);
		FreeVec(cs);
		return x;
		}
	return 0;

}

char Ans;
int TempChar;
int loop;
int chpos;

LONG main( LONG argc, TEXT **argv )
{
FILE *datafile;
char display_buffer[255];

	if(!(xport = (struct MsgPort *)FindPort(argv[1])))
		{
		printf("\nThis must be run from EXCELSIOR!\n");
		exit(30);
		}

	if(!(reply = (struct MsgPort *)CreateMsgPort()))
		{
		printf("\nCouldnt Create Repy port\n");
		exit(40);
		}

	if(!(cd = (struct Common_Data *)WhatIs(WHATIS_COMMONDATA)))
		{
		printf("\nNo common_data struct!\n");
		CloseDown(5);
		}

	if((datafile=fopen(header_file,"r")) == NULL)
		{
			PrintString("Couldn't open header file. Exiting...\n");
			CloseDown(0);
		}

	fgets(display_buffer,sizeof(display_buffer)-1,datafile);
	while(!feof(datafile))
		{
		PrintString(display_buffer);
	    fgets(display_buffer,sizeof(display_buffer)-1,datafile);
		}

	fclose(datafile);

	if((datafile=fopen(data_file,"r")) == NULL)
		{
			PrintString("Couldn't open data file. Exiting...\n");
			CloseDown(0);
		}

	for(loop=0; loop<12; loop++)
		{
			chpos=NULL;
			TempChar=1;

			while(TempChar!=NULL)
				{
					TempChar=fgetc(datafile);
					Wall[loop][chpos]=TempChar;
					chpos++;
				}
		}

	fclose(datafile);

	for(loop=0; loop<12; loop++)
		{
			PrintString(Wall[loop]);
			PrintString("\n");
		}

	if((datafile=fopen(footer_file,"r")) == NULL)
		{
			PrintString("Couldn't open footer file. Exiting...\n");
			CloseDown(0);
		}

	fgets(display_buffer,sizeof(display_buffer)-1,datafile);
	while(!feof(datafile))
		{
		PrintString(display_buffer);
	    fgets(display_buffer,sizeof(display_buffer)-1,datafile);
		}

	fclose(datafile);

	PrintString("\nWould you like to write in the Wall [Y/n]? ");
	Ans=GetCH();
	if(Ans=='n'|Ans=='N')
		{
			PrintString("\\c3No\n\\c5Have a nice day then. Humpf!\n");
			CloseDown(0);
		}
	PrintString("\\c3Yes\n\n\\c7Enter a short message for the Wall.\n:\\c6");
	GetLine(NULL,GL_NOANSI|GL_NOIUMS|GL_NOCMDSTACK,57);
	strcpy(Message,cd->Input_Buffer);
	if(strlen(Message)<3)
		{
			PrintString("\\c1Message too short. Nothing saved.\n");
			CloseDown(0);
		}
	strcpy(UserName,cd->U_Data->Alias);
	DataLine[0]=NULL;

	while(strlen(UserName)<14) strcat(UserName," ");
	while(strlen(Message)<57) strcat(Message," ");

	strcat(DataLine,"º\\c1 ");
	strncat(DataLine,UserName,14);
	strcat(DataLine," \\c7³\\c2 ");
	strncat(DataLine,Message,57);
	strcat(DataLine," \\c7º");

	for(loop=0; loop<11; loop++) strcpy(Wall[loop],Wall[loop+1]);
	strcpy(Wall[11],DataLine);

	if((datafile=fopen(data_file,"w")) == NULL)
		{
			PrintString("Couldn't open data file. Exiting...\n");
			CloseDown(0);
		}

	for(loop=0; loop<12; loop++)
		{
			chpos=NULL;
			TempChar=1;

			while(TempChar!=NULL)
				{
					TempChar=Wall[loop][chpos];
					fputc(TempChar,datafile);
					chpos++;
				}
		}

	fclose(datafile);

	PrintString("\\c1You painted THE WALL!\n");

	CloseDown(0);
}
