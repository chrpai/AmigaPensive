/*
 * EXCELSIOR! AmigaDOS		C "Door" interface include file
 * 
 * EXCELSIOR! BBS © 1992	Sycom Design Software, All Rights Reserved 
 *                       	Thomas Dietz,  Ronald Kushner
 *
 *
 *
 * This file is covered by the EXCELSIOR! program license agreement. 
 * Unauthorized distribution is prohibited.
 * 
 *
 *
 *
 $Header$
 *
 $Log: ex-c-doors.h,v $
 *
 * Revision 1.9  1992/07/18  11:13:22  tdietz
 * *** New System_Data layout, more expansion 
 * Revision 1.8  1992/07/03  14:25:48  rkushner
 * Revision 1.8  1992/07/03  14:25:48  rkushner
 * Revision 1.1  92/05/31  11:22:20  Unknown
 * 
 */

typedef struct IOExtSer  	IOSer;
typedef struct IOStdReq  	IOReq;
typedef struct IORequest 	IOrs;
typedef struct timerequest 	TIReq;
typedef struct MsgPort  	MSPort;
typedef struct List			LIST;
typedef struct Node 		NODE;
typedef struct MinNode 		MNODE;

#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <intuition/intuition.h>

#include <proto/exec.h>
#include <stdio.h>

#include "ex-userinfo.h"	  /* UserInfo Stuff */
#include "ex_flags.h"


struct Comm_Message   
    {
    struct  Message         message;
    ULONG   Command;
    ULONG   Pass1;
    ULONG   Pass2;
    ULONG   Pass3;
    ULONG   Pass4;
    ULONG   Pass5;
    ULONG   Pass6;
	ULONG 	Result1;
	ULONG 	Result2;
    };


struct System_Data
	{
	TEXT 	System_Name[41],
	     	Sysop_Name[21],
			LocalEditor[41]; /* Path and filename of local editor */	

	ULONG	Total_Users,    /* Total User Records Used */
	     	Active_Users,   /* Total User Still Active (ACTUAL USERS) */
            Serial_Number,  /* Highest Serial Number */
			Total_Calls,    /* Total Logged Calls */

			Base_Serial_Number, /* blah */ 
                
            System_Bits;    /* Defined bits on system operation */  	

	TEXT	System_Passwd[21], /* User Must Match This Before Login! */
            System_Font[32],   /* Font for BBS to use */
            Net_Address[41],   /* Network Address */
            Default_Language[21];      /* not in use */

	ULONG	Font_Size;		   /* The size of the font */
	
    struct ColorSpec Colors[17];     /* Sysop Definable Color Specs */

	UBYTE	Default_Color,
			Default_BarColor;

	TEXT	Path[15][128];		/* The System Path's */
								/* 0 = Data */
								/* 1 = Text */
								/* 2 = Mail */
								/* 3 = MBase */
								/* 4 = FBase */
								/* 5 = TBase */
								/* 6 = PBase */
								/* 7 = News */
								/* 8 = UUNEWS: */
								/* 9 = Main BBS */

	TEXT	Default_Country[4],				/* Default Country */
			Default_State[3],				/* Default State */
			Default_AreaCode[4];			/* Default Area Code */

 	ULONG	SetUp, 	/*  Setup Date */
			Reset;	/*  last reset date */

	ULONG 	NewUsers,			/*  how many newusers awaiting access */
			Call_Reset;			/*  how many calls since last reset */

	ULONG 	PostIDs;			/*  Serial ID of posted items */

	ULONG	MaxRooms;			/* Configured Conference Rooms */

	ULONG	Default_EdLines;	/* Default editor lines, this is for users */
								/* with UNLIMITED ed lines, there is no limit */
								/* just your memory */
								
	ULONG	Config;				/* Config Bits for the entire system */

	UWORD	Feedbacks,			/* New Feedbacks waiting to be read */
			Emails;				/* New emails waiting for ID#1 */ 

	ULONG	Total_Files,		/* Total Files on system */ 
			Total_Posts;		/* Total Posts on System */

	ULONG	FilesSince,			/* Files up'd since reset */
			PostsSince;			/* Posts since rest */

	ULONG	CaptureBufferSize;	/* Size for console buffer */

	UWORD	ShellPasswd;		/* The password encrypted for the shell */
	UWORD	padding;			/* 2 byte pad */

	ULONG	MaxBatchItems;
	ULONG	BatchHoldDays;

	TEXT	BatchPath[5];

	TEXT	System_Location[31],
			System_Number[31];

	ULONG	UserReset,
			exp1,
			exp2,
			exp3,
			flags;

	TEXT	SysopName[31],
			expansion1[81],
			expansion2[81],
			NetMailPath[81];

	ULONG	ScreenFlags;	/* The Screen bits for saving configurations */

	TEXT	Translation[24];
	ULONG	ExtFlags;			/* System Ext Flags */
	ULONG	CDROMPath;
	TEXT	QWKID[3];
	BYTE	buffer;
	TEXT	QWKIMPORT[8];
	ULONG 	Expansion[7];		/* expansion */
	};


struct Common_Data
	{
	struct	Int_Data 	*ID;	/* System Private */

	ULONG	Version;			/* Version number of this structure */

	BYTE	*CHL; 				/* Where did the last character return */
								/* from? 0 = unknown, 1=con, 2=ser */
	
	TEXT	**Ex;				/* Points to Excelsior Text */

	TEXT	*Input_Buffer;		/* Points to the Input Buffer */

	BOOL	*ANSI;				/* Is the user in ANSI mode? */

	LONG 	*Baud;				/* Baud rate the user is at */

	ULONG	*Login_Time;	/* Time user logged in */

	struct  UserInfo 	*U_Data,	/* User Data */
						*SU_Data;	/* Scratch UserData structure */

	ULONG	*User_ID;			/* The users ID on this node */
	
	BOOL	*IPLResponse;		/* IPL response, used after an IPL Yes/No  */

    struct  FileInfoBlock   *fib;   /* File Info Block allocated at startup */

	ULONG	*Info_Bits,			/* Bits for info on node REFER TO NODE_ defines*/
			*MoreInfo_Bits;		/* More bits.  REFER TO MNODE_ defines */  			

	struct	List 	*Editor;	/* Points to the editor */

	ULONG	*VisEdReturn;		/* 0 = No, 1 = Yes, 2 = return to std editor */

	ULONG	*MaxEditorLines;	/* The max lines available to enter */ 

	struct	System_Data		*Sys;	/* System Data Structure */

	TEXT	*ParseVal;			/* The parse val after a command */
								/* Not going to be preserved after a function called */

	ULONG	*MouseX,			/* Coordinates from our rastport */
			*MouseY;

	struct	List	*QUOTEs;			/* Quoted lines, if any */

	BOOL	*PrintMessage;		/* IPL response, used after an IPL Yes/No  */

	ULONG	*MaxLinesEntered;	/* The total number of lines entered */

	ULONG	*Ext_Flags;			/* More System Bits */

	ULONG	*Cur_Msg,			/* The following are private for Vised's */
			*Lo_Msg,
			*Hi_Msg,
			*TotalResponses;
	TEXT	*MiscString;		/* Pointer to buffer */
	};


/*
**  Mail Structure
*/

struct Mail_Data  
	{
    ULONG		Date_Sent,  /* Date Letter Was Sent */
                Date_Expire;/* Date Letter/Post expires */

    /* The Following is Good in case mail was forwarded */

    ULONG   To_ID,              /* The Recievers ID */
            To_Serial_Num;      /* The Recievers Serial Number */
    TEXT    To_Alias[61];       /* The Recievers Alias */

    ULONG   From_ID,   			/* The Senders User ID */
			From_Serial_Num;    /* Senders Serial Number */
    TEXT    From_Alias[101];     /* Senders Alias */

    ULONG   Next_Letter;

    ULONG   Last_Letter;

    UWORD   Mess_Flags;  /*  All The Flags For Mail */

    TEXT    Subject[61];  /*  The Message Subject */

	ULONG	CarbonCopies[75];	/* Carbon Copies */

	TEXT	UUCPDate[128];	/* Date For UUCP Messages */
	};


/*
**  Editor Line Structure
*/

struct ED_Node
	{
	MNODE	node;			/* Minnode structure */
	TEXT 	*line;			/* pointer to editor line for this node */
	BYTE	Flags;			/* Only eight flags allowed */
	};
