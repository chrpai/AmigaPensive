#ifndef	LIBRARIES_DDDOOR_H
#define	LIBRARIES_DDDOOR_H	1

/*
**	$Filename: libraries/dddoor.h $
**	$Release: 1.00 $
**	$Revision: 1.0 $
**	$Date: 96/10/10 $
**
**	DDDoor library name and useful definitions.
**
*/

#ifndef	EXEC_TYPES_H
#include	<exec/types.h>
#endif

#ifndef	EXEC_LIBRARIES_H
#include	<exec/libraries.h>
#endif

#ifndef DAYDREAM_H
#include <daydream.h>
#endif

/*
 ************************************************************************
 *	Standard definitions for DreamDoor library information		*
 ************************************************************************
 */

#define	DDDoorName		"dreamdoor.library"

/*
 ************************************************************************
 *									*
 *	The Door InterFace data structure...				*
 *									*
 ************************************************************************
 */

struct DIFace	{
	APTR	dif_DDPort;		// Ptr to DayDream port to send msg to
	APTR	dif_ReplyPort;		// Ptr to our DoorReplyPort to receive msg
	APTR	dif_Message;		// Ptr to an initialized DDDP message
	APTR	dif_Buffer;		// Ptr to private buffer (DO NOT USE!!)
	};

struct DDPointers  {
	ULONG	dp_Connecttype;					// 1 = Remote, 0 = Local
	struct	MsgPort *dp_Server;				// Pointer to Server MSG-port
	struct  DayDream_Archiver *dp_Archivers;		// Pointer to Archivers.DAT
	struct  DayDream_MainConfig *dp_DayDream;		// Pointer to DayDream.DAT
	struct	DayDream_DisplayMode *dp_Display;		// Pointer to Display.DAT
	struct	DayDream_DisplayMode *dp_CurrDisplay;		// Pointer to current display mode structure
	struct  DayDream_Conference *dp_Conferences;					// Pointer to Conferences.DAT
	struct	DayDream_Conference *dp_CurrConf;		// Pointer to current Conference structure
	struct  DayDream_MsgBase *dp_CurrBase;			// Pointer to current Message base structure
	struct  DayDream_Protocol *dp_Protocols; 		// Pointer to Protocols.DAT
	struct  DayDream_User *dp_CurrUser; 			// Pointer to current user
	struct	DayDream_ExternalCommand *dp_Externals;		// Pointer to External Commands
	struct	DayDream_Schedule *dp_Schedule;			// Pointer to Schedule.DAT
	APTR 	dp_DoorParams;					// Pointer to Door parameters
	ULONG	dp_BpsRate;					// Current BPS Rate
	struct	NodeStruct *dp_NodeStructure;			// Pointer to node structures
	struct	NodeStruct *dp_CurrentNode;			// Pointer to node structure of this node
	APTR	dp_InComingTable;				// Pointer to incoming conversion table
	APTR	dp_OutGoingTable;				// Pointer to outgoing conversion table
	struct	DD_Seclevel *dp_Security;			// Pointer to security.dat
	APTR	dp_Selected;					// Pointer to Selected.DAT
	struct	DayDream_IO_Device *dp_Iodev;			// Pointer to IO-Device of the current node
	APTR	dp_DoorCmd;					// Pointer to Door command
	struct	DayDream_Multinode *dp_Multinodes;		// Pointer to Multinodes.DAT
	ULONG	dp_Lastcall;					// Last call (secs since 78)
        struct  List *dp_FileChain;
	char	dp_Future[38];
};


#define DPF_HIDDEN (1L<<0)
#define DPF_NOCRLF (1L<<1)
#define DPF_FILENAME (1L<<2)

#define DDF_ADDEXTENSION (1L<<0)
#define DDF_SHOWERROR (1L<<1)
#define DDF_STRIPKLUDGES (1L<<2)
#define DDF_CHECKCONF (1L<<3)
#define DDF_CHECKACS (1L<<4)
#define DDF_STRIPCTRL (1L<<5)

#define DYESNO_NO 0
#define DYESNO_YES 1
#define DYESNO_ERROR -1

#define HOTKEY_SHOWCHAR (1L<<0)
#define HOTKEY_ADDLF	(1L<<1)
#define HOTKEY_CURSOR	(1L<<2)
#define HOTKEY_DETECT	(1L<<3)

#define FLAGFILE_FREE   (1L<<0)

#define JOINCONFF_ASKNEW (1L<<0)
#define JOINCONFF_QUICK  (1L<<1)
#define JOINCONFF_SKIPTXT (1L<<2)

#define CMBF_SHOWMENU (1L<<0)
#define CMBF_NOSTATUS (1L<<1)

#endif
