/********************************************************************************************************************************
 *                                                                                                                              *
 * NOViA - Interactive Network Solutions                                                                                        *
 *                                                                                                                              *
 * MainServer                                                                                                                   *
 *                                                                                                                              *
 * Copyright (c) 1996,1997,1998,1999 Thorsten Gehler/UFoP                                                                       *
 *                                                                                                                              *
 * Release: Amiga            Server Software 0.01.03 beta                                                                       *
 *          NetBSD 1.32      Server Software 0.00.01 alpha                                                                      *
 *          Apple Macintosh  Client Software 0.00.00 alpha                                                                      *
 *                                                                                                                              *
 ********************************************************************************************************************************/

#include <pragma/gadtools_lib.h>
#include <pragma/rexxsyslib_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <pragma/commodities_lib.h>
#include <libraries/commodities.h>
#include <clib/dos_protos.h>
#include <pragma/intuition_lib.h>
#include <pragma/graphics_lib.h>
#include <utility/utility.h>
#include <pragma/utility_lib.h>
#include <exec/semaphores.h>

#include <devices/timer.h>
#include <intuition/intuition.h>
#include <novia/novia_gui.h>
#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_funcs.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <dos/dostags.h>
#include <dos/stdio.h>
#include <string.h>
#include <pragma/noviasys_lib.h>

#include "novia_reg_data.h"

//#define DEBUG

#define NOVIA_VERSION "0.74"

char ver[]="$VER: NOVIA NETWARE (AMIGA) "NOVIA_VERSION" ("__DATE2__" "__TIME__"), copyright © 1996,1997,1998,1999 by Thorsten Gehler";

struct NewMenu mymenu[] =
{
	{ NM_TITLE, "Project",			0,	0, 0, 0,},
	{  NM_ITEM,		"About...",		"?",0, 0, 0,},
	{  NM_ITEM,		NM_BARLABEL,	0,	0, 0, 0,},
	{  NM_ITEM,		"Lock consule",	0,	0, 0, 0,},
	{  NM_ITEM,		"Close window",	0,	0, 0, 0,},
	{  NM_ITEM,		NM_BARLABEL,	0,	0, 7, 0,},
	{  NM_ITEM,		"Restart BBS",	0,	0, 0, 0,},
	{  NM_ITEM,		"Reset Server",	0,	0, 0, 0,},
	{  NM_ITEM,		NM_BARLABEL,	0,	0, 0, 0,},
	{  NM_ITEM,		"Quit",			"Q",0, 0, 0,},
	{ NM_TITLE, "Services",			0,	0, 0, 0,},
	{  NM_ITEM,		"All Server off",	0,	CHECKIT | MENUTOGGLE | CHECKED, 0, 0,},
	{  NM_ITEM,		"No answer",		0,	CHECKIT | MENUTOGGLE | CHECKED, 0, 0,},
	{  NM_ITEM,		"Enter Sysop chat",	"c",	0, 0, 0,},
	{  NM_ITEM,		"hanging up",	"h",	0, 0, 0,},
	{   NM_END, NULL,				0,	0, 0, 0,}
};

void c_FreeClientPort();
struct PortData *CreateClient(PortData *cport);
struct PortData *c_init();
struct MsgPort *reminder_eventport=NULL;
struct MsgPort *main_eventport=CreateMsgPort();

long GetCommandID(char *string);
void SaveMainPortConfig( MainPortConfig *pointer);
void CloseIt( char error_code, char *error_text);
struct MainPortConfig *LoadMainPortConfig(MainPortConfig *pointer);
LONG m_init();
void m_StartPort(); 		// Prototype
void c_StartPort(); 		// Prototype
BOOL CloseClientPort(LONG PortID);
BOOL SendGUIMsg(LONG msgid,APTR data);
void ReturnGUIMsg(GUIMsg *msg);
void FreeGUIMsg(GUIMsg *msg);
void LoadClientList();
void SaveClientList();
struct PortData *NewClientPort();
void SaveClientConfig( ClientConfig *cconfig);

void MyFreeVec( APTR address,  char *text);
APTR MyAllocVec( ULONG size,  ULONG flags,  char *text);
void LoadMCICodes(ULONG *numberofmci, struct mcicode *mcicmd);
void LoadCommandList(Directory *cmddir);
BOOL LoadMemberList(char *filename, List *ulist);
BOOL SaveMemberList(char *filename, List *ulist);
void RepairUserList( UserList *ulist);
void LoadStringTable();
void RemoveStringList();
struct PortData *FindClient(ULONG PortID);
void StartGUI();
void nrc_start();
void nfs_start();
void LoadEvents();
void SaveEvents();
void LoadMailServers();
void SaveMailServers();
void mystrcpy(char *dest,char *src,ULONG length);
void StartReminder();
void reminder_main();
ULONG CalcEvent(Event *event);
void ExecuteEvent(Event *event);
void SendOnlineMessage(PortData *cport);
PortData *FindClient( ULONG PortID);
PortData *FindClientByUser( ULONG UserID);
void Writechar(char i);
void CR();
extern void LowLevelCommand_makefs();

extern BOOL OpenXPrWindow();
extern void WaitConnect();
extern void CloseXPrWindow();
extern BOOL DoTransfer();
extern void DoRexxMsg(RexxMsg *rexxmsg);
extern void gui_main();
extern void nfs_main();


struct Library *nslib_LibOpen();
struct Library *nslib_LibClose();
struct Library *nslib_LibExpunge();
struct Library *nslib_LibExtfunc();
struct PortData *nslib_InitPort();
struct PortData *nslib_ClosePort();


extern void FreeNRCList();
extern void LoadNRCList(List *);
extern void EditNRCList(List *);
extern void SaveNRCList(List *);
extern void SaveNRC(struct Room *room);
extern void LoadNRC(struct Room *room);
extern void KillNRC(struct Room *room);
extern void nrc_main();
extern void OpenConference();
extern ULONG searchstr(char *str1, char *str2);
extern struct EventMessage *SendEventMsg(MsgPort *sendport, MsgPort *replyport, APTR data, ULONG msg_type);
extern void RedrawEventList();

extern LONG LowLevelCommand_makefilesystem();
extern LONG LowLevelCommand_makeroot();
extern LONG LowLevelCommand_adddirectory(char *name);
extern LONG LowLevelCommand_addlink(char *name);
extern LONG LowLevelCommand_addcommand(char *name);

/**** LIBRARIES BASES ***/

struct Library *GadToolsBase	= NULL;
struct RxsLib *RexxSysBase		= NULL;
struct Library *TimerBase		= NULL;
struct Library *NoviaSysBase	= NULL;
struct Library *MUIMasterBase	= NULL;
struct Library *UtilityBase	= NULL;
struct Library	*CxBase			= NULL;

/*** MESSAGEPORT NAMES ***/

char *MainPortName				= "Novia_MainPort";
char *MainGUIPortName			= "Novia_GUIMainPort";
char *GUIPortName					= "Novia_GUI_Frontend_Port";

char *Titel="NOVIA-Netware, Copyright © 1996-99 by Thorsten Gehler Version "NOVIA_VERSION" preview alpha";


/*** PRIVATE LISTS ***/


struct NRCServer *nrcserver=NULL;

struct List *lock_list			= NULL,							// locked list
				*client_list		= NULL;

struct MsgPort *MainGUIPort	= CreateMsgPort();
struct MsgPort *GUIPort			= NULL;

ULONG  numberofclients			= 0;
struct MainPortConfig *mainportconfig = NULL;
struct Task *MainTask			= FindTask(NULL);

struct GUIFrontend guifrontend;
struct Task *GUITask				= NULL;

struct Date actualdate;
struct NFSServer *nfsserver	= NULL;


/*** COMMODITY ***/

struct MsgPort 	*cxport		= NULL;
CxObj					*broker		= NULL;

#define PRIORITY_TOOL_TYPE			"CX_PRIORITY"
#define POP_ON_START_TOOL_TYPE	"CX_POPUP"
#define POPKEY_TOOL_TYPE			"CX_POPKEY"
#define POP_KEY_ID     (86L)     /* pop up identifier             */
#define POP_KEY_DEFAULT				"RALT SHIFT DEL"

#define COM_NAME						"Novia"
#define COM_TITLE						"Novia V 0.06"
#define COM_DESCR						"NOViA Interactive Network Services"

struct NewBroker mynb = {
   NB_VERSION,                        /* Library needs to know version */
  	COM_NAME,                          /* broker internal name          */
   COM_TITLE,                         /* commodity title               */
  	COM_DESCR,                         /* description                   */
   NBU_NOTIFY | NBU_UNIQUE,           /* We want to be the only broker */
                                     /* with this name and we want to */
                                     /* be notified of any attempts   */
                                     /* to add a commodity with the   */
                                     /* same name                     */
   0,                                 /* flags                         */
  	0,                                 /* default priority              */
   NULL,                              /* port, will fill in            */
  	0                               /* channel (reserved)            */
};

/*** RUNTIME SEMAPHORE ***/

struct SignalSemaphore *rt_semaphore		= NULL;
char 						  *rt_semaphore_name = "Novia_RTOS_Semaphore";

/********************/

void main(int argc, char **argv)
{
	LONG returncode=m_init();
	if (!returncode)
		m_StartPort();
	CloseIt(returncode,"");
}

void c_main()
{
	struct PortData *cport=c_init();
	if (cport)
	{
		while (!cport->ProgramClose)
		{
			SN_WaitConnect();
			c_StartPort();
		}
	}
	c_FreeClientPort();

}


void wbmain()
{
	main(0,0);
}

void mainwb()
{
	main(0,0);
}


LONG m_init()
{
	LONG returncode=NULL;
	struct PortData *cport;
	guifrontend.ProgramClose=0;
	guifrontend.GUIOpen=0;
	if ((cport = AllocVec(sizeof(PortData), MEMF_ANY | MEMF_CLEAR)))
	{
		BPTR mulder;
		ULONG entrys;
		ULONG counter;
		struct GUIMsg *waitmsg;
		BOOL quit=FALSE;
		char buffer[200];
	
		MainTask->tc_UserData	= cport;
//		cport->network.isopen	= TRUE;
		cport->network.online	= TRUE;
//		cport->network.can_read	= TRUE;
//		cport->network.can_write= TRUE;

		CreateNoviaLibrary();
//		NoviaSysBase = OpenLibrary("noviasys.library",0);
		if (NoviaSysBase)
		{
			strcpy(cport->RexxPortName, "NOVIAREXX0");
			strcpy(cport->TaskName, "novia_main");
			cport->ConPort		= CreateMsgPort();
			cport->MainPort	= CreateMsgPort();
			cport->LocalPort	= cport->MainPort;
			cport->rexxport	= CreateMsgPort();
			cport->rtport		= CreateMsgPort();
			cport->nfs_port	= CreateMsgPort();

			if (cport->MainPort && cport->ConPort && cport->rexxport && cport->rtport && cport->nfs_port)
			{
				cport->MainPort->mp_Node.ln_Name	= MainPortName;
				cport->MainPort						= cport->MainPort;
				AddPort(cport->MainPort);
			}
			else
			{
				printf("can't create MainPort.\n");
				returncode = 20;
			}
		}
		else
		{
			printf("Can't create noviasys.library.\n");
			returncode = 20;
		}


		if (!returncode)
		{
			if (cport->ConPort)
			{
				cport->ConRead		= CreateIORequest(cport->ConPort, sizeof(IOStdReq));
				cport->ConWrite	= CreateIORequest(cport->ConPort, sizeof(IOStdReq));
				if (!cport->ConRead | !cport->ConWrite)
				{
					returncode = 20;
					printf("Can't create IORequest-structure for console.device\n");
				}
			}
			else
			{
				printf("Can't create MSG-Port for console.device\n");
				returncode=20;
			}
		}
		if (!returncode)
		{
			if ((cport->TimePort = CreateMsgPort()))
			{
				if ((cport->TimerReq = CreateIORequest(cport->TimePort, sizeof(timerequest))))
					if (OpenDevice("timer.device", UNIT_VBLANK, (IORequest *)cport->TimerReq, 0))
					{
						printf("can't open timer.device.\n");
						returncode = 20;
					}
					else
					{
						TimerBase = (struct Library *)cport->TimerReq->tr_node.io_Device;
						if ((!cport->TimerReq)|(!TimerBase))
						{
							printf("can't open timer.device.\n");
							returncode = 20;
						}
					}
			}
			else
			{
				printf("Can't create MSG-Port for timer.device\n");
				returncode=20;
			}
		}

		if (!returncode)
		{
			if (!(GadToolsBase = OpenLibrary("gadtools.library", 37)))
			{
				printf("Can't open gadtools.library.\n");
				returncode = 20;
			}
		}

		if (!returncode)
		{
			if (!(RexxSysBase = (RxsLib *)OpenLibrary(RXSNAME, 0)))
			{
				printf("Can't open rexxsyslib.library.\n");
				returncode = 20;
			}
		}

		if (!returncode)
		{
			if (!(UtilityBase = OpenLibrary("utility.library",37)))
			{
				printf("Can't open utility.library.\n");
				returncode = 20;
			}
		}

#ifdef DEBUG
		printf("start cx commodity\n");
		Delay(200);
#endif

		if (!returncode)
		{
		   if ((CxBase = (struct Library *)OpenLibrary("commodities.library",5)))
			{
			   if ((cxport = CreateMsgPort()))
				{
					mynb.nb_Flags |= COF_SHOW_HIDE;										// Enable SHOW/HIDE gadgets
					mynb.nb_Port	= cxport;
					if ((broker = CxBroker(&mynb, NULL)))								// Attempt to create our broker
					{
						AttachCxObj(broker, HotKey(POP_KEY_DEFAULT, cxport,POP_KEY_ID));	// install a hotkey for popping up window. Setup all application specific commodities objects
//						if (error = CxObjError(broker))

					   ActivateCxObj(broker,1L);												// All went well so activate our broker
					}
				}
			}
		}

#ifdef DEBUG
		printf("start title\n");
		Delay(200);
#endif

		if (!returncode)
		{
			cport->Titel = Titel;
//			strcpy(cport->actitel, Titel);
			if (!(cport->iobuffer = AllocVec(10000, MEMF_ANY | MEMF_CLEAR)))
			{
				printf("can't alloc mem for iobuffer.\n");
				returncode = 20;
			}	
		}
		MUIMasterBase = OpenLibrary(MUIMASTER_NAME,0);

		NewList((List *)&cport->iobuffer_list);
		NewList((List *)&cport->his_list);
		NewList((List *)&cport->select_list);
		NewList((List *)&cport->downloadlist);
		NewList((List *)&cport->rexxlist);
		NewList((List *)&cport->itementrylist);

#ifdef DEBUG
		printf("set program name\n");
		Delay(200);
#endif

		SetProgramName(cport->TaskName);

		if (!returncode)
		{
			if (!(client_list = AllocVec(sizeof(List), MEMF_ANY | MEMF_CLEAR)))
			{
				printf("Can't create client_list.\n");
				returncode = 20;
			}
			else
				NewList(client_list);
		}

		/************************************************************************/
		/***                  starting graphics user interface                ***/
		/************************************************************************/
	
#ifdef DEBUG
		printf("check gui\n");
		Delay(200);
#endif

		if (!returncode)
		{
			if (MainGUIPort)
			{
				MainGUIPort->mp_Node.ln_Pri	= 0;
				MainGUIPort->mp_Node.ln_Name	= MainGUIPortName;
				AddPort(MainGUIPort);
			}
			else
			{
				returncode = 20;
				printf("Can't create MainGUIPort.\n");
			}
		}

#ifdef DEBUG
		printf("mainportconfig\n");
		Delay(200);
#endif

		if (!returncode)
		{
			if (!(mainportconfig = AllocVec(sizeof(MainPortConfig), MEMF_ANY | MEMF_CLEAR)))
			{
				printf("Can't create MainPortConfig-structure.\n");
				returncode=20;
			}
			else
			{

				#ifdef DEBUG
					printf("load mainportconfig\n");
					Delay(200);
				#endif
				if (LoadMainPortConfig(mainportconfig))
				{
					cport->sysdirs							= &mainportconfig->sysdirs;
					cport->clientconfig.ScreenModeID	= mainportconfig->ScreenMode;
					cport->clientconfig.UseWorkbench	= mainportconfig->CH_Main;
					cport->clientconfig.Colors			= mainportconfig->Colors;
				}
				else
				{
					if (MUIMasterBase)
					{
						struct RegApp *regapp=NULL;
						if ((regapp = CreateRegApp()))
						{
							{
								BOOL	running = TRUE;
								ULONG	signal;
								ULONG	result=0;
								char *buf=NULL;
								while (running)
								{
									if ((result=DoMethod(regapp->App,MUIM_Application_Input,&signal)))
									{
										switch (result)
										{
											case MUIV_Application_ReturnID_Quit:	case REGID_SAVE:

												get(regapp->STR_RegNumber,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegID,buf,20);

												get(regapp->STR_Reg_Name,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegName,buf,21);

												get(regapp->STR_Reg_Location,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegLocation,buf,81);

												get(regapp->STR_Reg_Voicenumber,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegVoicenumber,buf,41);

												get(regapp->STR_Reg_Modem,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegModem,buf,41);

												get(regapp->STR_Reg_EMail,MUIA_String_Contents,(APTR)&buf);
												strncpy(mainportconfig->RegEMail,buf,25);

												strcpy(mainportconfig->sysdirs.sysdir,"novia:");
												strcpy(mainportconfig->sysdirs.userdir,"novia:users/");
												strcpy(mainportconfig->sysdirs.sysdata,"novia:sysdata/");
												strcpy(mainportconfig->sysdirs.systext,"novia:systext/");
												strcpy(mainportconfig->sysdirs.newuser,"novia:newuser/");

												mainportconfig->CH_Main=TRUE;
												mainportconfig->Colors=8;
												mainportconfig->LoadGUI=TRUE;

												cport->clientconfig.UseWorkbench=TRUE;
												cport->clientconfig.Colors=8;

												SaveMainPortConfig(mainportconfig);

												running=FALSE;
												returncode=0;
												break;
										}
									}
									if(running && signal) Wait(signal);
								}
							}
							DisposeRegApp(regapp);
							/*
							strcpy(pointer->sysdirs.sysdir,"novia:");
							strcpy(pointer->sysdirs.userdir,"novia:users/");
							strcpy(pointer->sysdirs.sysdata,"novia:sysdata/");
							strcpy(pointer->sysdirs.systext,"novia:systext/");
							strcpy(pointer->sysdirs.newuser,"novia:newuser/");
							*/
						}
						else
						{
							printf("Can't create registration application.\n");
							returncode=20;
						}
					}
					else
					{
						printf("Can't open muimaster.library.\n");
						returncode=20;
					}
				}
				mainportconfig->rexxtempcounter	= 0;
			}
		}
		/************************************************************************/
	
		if (!returncode)
		{
			cport->mymenu			= mymenu;
			cport->Supervisor		= TRUE;
			cport->NewUser			= TRUE;
			cport->Dumped			= FALSE;
			cport->HideAll			= FALSE;
			cport->MuffAll			= FALSE;
			cport->MonitorAll		= FALSE;
			cport->UL.UserEntrys	= 0;
			cport->PortStatus		= PORT_STATUS_ONLINE;
			cport->systemservice	= TRUE;

#ifdef DEBUG
		printf("monitor_open\n");
		Delay(200);
#endif

			Monitor_open();

#ifdef DEBUG
		printf("show welcome\n");
		Delay(200);
#endif

			ShowText("work:novia/systext/sys.novia.text");

			NewList((List *)&mainportconfig->EventList);
			NewList((List *)&mainportconfig->LocalMailServerList);
			NewList((List *)&mainportconfig->POP3ServerList);

#ifdef DEBUG
		printf("nrc start\n");
		Delay(200);
#endif

			nfs_start();
			ioprintf("starting Novia Mail Services");

#ifdef DEBUG
		printf("Load Mail Server\n");
		Delay(200);
#endif

			LoadMailServers();
			CR();ioprintf("[70Cc2donec7\n");
			ioprintf("setting up Eventlist");

#ifdef DEBUG
		printf("Load Events\n");
		Delay(200);
#endif

			LoadEvents();
			CR();ioprintf("[70Cc2donec7\n");
			ioprintf("setting up StringTable");

#ifdef DEBUG
		printf("Load string table\n");
		Delay(200);
#endif

			LoadStringTable();
			CR();ioprintf("[70Cc2donec7\n");

#ifdef DEBUG
		printf("load user list\n");
		Delay(200);
#endif

			LoadUserList(&cport->UL);
			if ((cport->cmddir = AllocVec(sizeof(Directory), MEMF_ANY | MEMF_CLEAR)))
			{
				LoadCommandList(cport->cmddir);
			}
			LoadMCICodes(&mainportconfig->numberofmci, mainportconfig->mcicmd);
			cport->mcicmd			= mainportconfig->mcicmd;
			cport->numberofmci	= &mainportconfig->numberofmci;
			ioprintf("looking for client settup.");
			CR();ioprintf("[70Cc2donec7\n");

#ifdef DEBUG
		printf("load client list\n");
		Delay(200);
#endif

			LoadClientList();
		}

		if (!returncode)
		{
			if (cport->rexxport)
			{
				cport->rexxport->mp_Node.ln_Pri	= 0;
				cport->rexxport->mp_Node.ln_Name	= cport->RexxPortName;
				AddPort(cport->rexxport);
			}
			else
			{
				printf("can't create rexxport.\n");
				returncode=20;
			}
		}

		if (!returncode)
		{
//			ndos_loadselectlist((List *)&cport->select_list);
			ioprintf("starting Novia Relay Chat Services");
//			nrc_start();
			CR();ioprintf("[70Cc2donec7\n");
			if (mainportconfig->LoadGUI)
			{
				ioprintf("starting Graphic User Interface\n");
				StartGUI();
			}
			if (main_eventport)
			{

#ifdef DEBUG
		printf("start reminder\n");
		Delay(200);
#endif
				ioprintf("start Reminder");
				StartReminder();
				CR();ioprintf("[70Cc2donec7\n");
			}
		}
		if (!returncode)
		{
			if (!(cport->inputbuffer = AllocVec(256,MEMF_ANY|MEMF_CLEAR)))
				returncode = 20;
		}
		if (!returncode)
		{
			if ((rt_semaphore = AllocVec(sizeof(struct SignalSemaphore), MEMF_ANY | MEMF_CLEAR)))
			{
				rt_semaphore->ss_Link.ln_Name = rt_semaphore_name;
				InitSemaphore(rt_semaphore);
				AddSemaphore(rt_semaphore);
			}
			else
			{
				ioprintf("can't alloc mem for rt_semaphore\n");
				returncode = 20;
			}
		}
	}
	else
	{
		printf("Can't create PortData-Structure.\n");
		returncode=20;
	}
	return returncode;
}


void CloseIt( char error_code, char *error_text)
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	struct Task *reminder_task=NULL;
	if (cport)
	{
		ULONG quit=FALSE;
		struct PortData2 *nc=(PortData2 *)client_list->lh_Head;
		while (nc->port_node.ln_Succ)
		{
			if (nc->port_address->PortStatus>0)
			{
				ioprintf("c3closing Port: c7%d.\n",nc->port_address->clientconfig.IDNumber);
				SendMsg(nc->port_address->LocalPort,nc->port_address,0,MSG_PORT_CLOSE,0);
				nc->port_address->ProgramClose=TRUE;
				nc->port_address->network.online=FALSE;
				if (nc->port_address->task)
					Signal(nc->port_address->task,nc->port_address->task->tc_SigWait);
			}
			nc=(PortData2 *)nc->port_node.ln_Succ;
		}

		while (!quit)
		{
			quit=TRUE;
			nc=(PortData2 *)client_list->lh_Head;
			while (nc->port_node.ln_Succ && quit)
			{
				if (nc->port_address->PortStatus>0)
					quit=FALSE;
				nc=(PortData2 *)nc->port_node.ln_Succ;
			}
			Delay(5);
		}



		if (guifrontend.GUIOpen && GUITask)
		{
			guifrontend.ProgramClose=TRUE;
			Signal(GUITask,GUITask->tc_SigWait);
			ioprintf("wait for closeing GUI Sig\n");
			while (guifrontend.GUIOpen)
			{
				Delay(5);
			}
		}


		if (cport->currentdir)
			nfs_support_closeroot(cport);

		if (main_eventport)
		{
			if ((reminder_task=FindTask("novia_reminder")))
			{
				if (SendEventMsg(reminder_eventport,main_eventport,0,EVENTMSGTYPE_QUIT))
				{
					struct EventMessage *msg;
					quit=FALSE;
					while (!quit)
					{
						if ((msg=(EventMessage *)GetMsg(main_eventport)))
						{
							if (msg->msg_type==EVENTMSGTYPE_QUIT)
							{
								quit=TRUE;
							}
							if (msg->msg.mn_ReplyPort==main_eventport)
							{
								FreeVec(msg);
							}
						}
						Delay(5);
					}
					DeleteMsgPort(main_eventport);
				}
			}
			ioprintf("reminder closed.\n");
		}

		if (nfsserver)
		{
			if (nfsserver->nfstask)
			{
				nfsserver->CloseServer = TRUE;
				Signal(nfsserver->nfstask,nfsserver->nfstask->tc_SigWait);
				while(nfsserver)
					Delay(5);
			}
		}

		if (cport->inputbuffer)
		{
			FreeVec(cport->inputbuffer);
			cport->inputbuffer = NULL;
		}
		if (nrcserver)		// Shutdown nrcserver
		{
			UWORD counter=0;
			ioprintf("close nrc-server.\n");
			nrcserver->ServerClose=TRUE;
			Signal(nrcserver->nrctask,1<<nrcserver->controlport->mp_SigBit);
			while (nrcserver)
			{
				Delay(5);
			}
			ioprintf("nrc-server closed.\n");
		}

//		ClearList((List *)&cport->select_list);
		ClearList((List *)&cport->his_list);
		ClearList((List *)&cport->iobuffer_list);

		if (cport->rexxport)
		{
			RemPort(cport->rexxport);
			DeleteMsgPort(cport->rexxport);
		}	


//		c_FreeClientList();

		if (cport->mcicmd)FreeVec(cport->mcicmd);
		*cport->numberofmci=0;
		if (cport->cmddir)
		{
			CommandEntry *commandentry;
			while (!IsListEmpty((List *)&cport->cmddir->entrylist))
			{
				commandentry = (CommandEntry *)cport->cmddir->entrylist.mlh_Head;
				Remove((Node *)commandentry);
				FreeVec(commandentry);
			}
			FreeVec(cport->cmddir);
		}

		if (cport->UL.UserEntrys && cport->UL.utable)
		{
			FreeVec(cport->UL.utable);
			cport->UL.UserEntrys=0; cport->UL.utable=NULL;
		}

		Monitor_close();

		if (MainGUIPort)
		{
			RemPort(MainGUIPort);
			DeleteMsgPort(MainGUIPort);
		}
		
		if (client_list)FreeVec(client_list);
		if (cport->iobuffer)FreeVec(cport->iobuffer);
		
		if (RexxSysBase)CloseLibrary((Library *)RexxSysBase);
		if (GadToolsBase)CloseLibrary((Library *)GadToolsBase);
		if (MUIMasterBase)CloseLibrary((Library *)MUIMasterBase);
		if (UtilityBase)CloseLibrary((Library *)UtilityBase);

		if (cport->TimerReq)
		{
			if (cport->TimerReq->tr_node.io_Device)
				CloseDevice((IORequest *)cport->TimerReq);
			DeleteIORequest((IORequest *)cport->TimerReq);
		}

		if (cport->TimePort)
			DeleteMsgPort(cport->TimePort);

		if (cport->ConRead)DeleteIORequest((IORequest *)cport->ConRead);
		if (cport->ConWrite)DeleteIORequest((IORequest *)cport->ConWrite);
		if (cport->ConPort)DeleteMsgPort(cport->ConPort);
		if (cport->MainPort)
		{
			RemPort(cport->MainPort);
			DeleteMsgPort(cport->MainPort);
		}
		if (cport->rtport)
			DeleteMsgPort(cport->rtport);
		RemoveStringList();

		if (NoviaSysBase) RemoveNoviaLibrary(NoviaSysBase);
		if (mainportconfig)FreeVec(mainportconfig);
		if (cport->nfs_port) DeleteMsgPort(cport->nfs_port);
		FreeVec(cport);

		if (broker)DeleteCxObjAll(broker);
		if (cxport)DeleteMsgPort(cxport);

		MainTask->tc_UserData = NULL;

		if (rt_semaphore)
		{
			RemSemaphore(rt_semaphore);
			FreeVec(rt_semaphore);
		}
	}
	if (error_code)printf("FATAL ERROR: %s\n",error_text);
//	Exit((int)error_code);
}

void m_StartPort()
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	char buffer[200];
	char Quit=FALSE;
	char Cancel=FALSE;
	ULONG Usernummer = 1,
			user2;
	char counter=0;
	ULONG ulong,cmdid;
	BOOL nostdcmd;		// no standart command
	struct PortData *cport2;
	struct PortData2 *nc;
	struct Room *room;
	BPTR mulder;
	Writeio("",-1);
	sprintf(buffer,"%ssys.start.new",cport->sysdirs->systext);
	ShowText(buffer);LF();
	ioprintf(&ver[6]);LF();
	ioprintf("Copyright (c)1996,1997,1998,1999 by Thorsten Gehler. All rights reserved.\n\n");
	
	ChangeScreenStatus("LOCKED");

	cport->LocalUser.Flag_supervisor=TRUE;
	while (!Quit && counter<5 && !cport->ProgramClose && cport->network.online)
	{
		Writeio("Enter System Password :",-1);
		Getstring(buffer,0,20,0,0);
		Writeio("Enter         Encrypt :",-1);
		ChangeScreenStatus("ONLINE");
		Getstring(buffer,0,20,0,0);
		{
			struct ItemEntryBlock *ieb;
			ioprintf("\nUpdating System Database... please wait.\n\n");
			LoadUser(&cport->LocalUser,Usernummer);
			if (ndos_openroot())
				ioprintf("root opened\n");
			ndos_loadselectlist((List *)&cport->select_list);
//			ndos_getlist();
			sprintf(buffer,"%ssys.welcome.new",cport->sysdirs->systext);
			ShowText(buffer);LF();
			Quit = FALSE;
			while (!Quit && !cport->ProgramClose && cport->network.online)
			{
				ioprintf("\nc3%d.%s> c7",cport->clientconfig.IDNumber,cport->currentdirname);
				Getstring(cport->inputbuffer,0,80,BUFFERED,0);
				GetArg(cport->inputbuffer);
				if (strlen(cport->argstring[0]))
				{
					if ((ieb = ndos_get_ieb(cport->argstring[0])))
					{
						switch (ieb->ItemType)
						{
							case ITEMTYPE_SUBBOARD:
								ndos_changedir(ieb->filename);
								break;
							case ITEMTYPE_MESSAGE:
								ndos_readmessage(ieb);
								break;
							case ITEMTYPE_FILEARCHIV:
								ioprintf("%s: Unknown object type\n",cport->argstring[0]);
								break;
							case ITEMTYPE_EMAIL:
//								ioprintf("%s: Unknown object type\n",cport->argstring[0]);
								ioprintf("readmailboxerror: %d\n",ndos_readmailbox(ieb));
								break;
							case ITEMTYPE_COMMAND:
								{
									ndos_execute(cport->argstring[0]);
								}
								break;
						}
					}
					else
					{
						if (cport->inputbuffer[0] == '*')
						{
							ndos_tstselect(cport->inputbuffer);
						}
						else
						{
							if (*cport->argstring[0] == '/')
							{
								char *ptr = cport->argstring[0] + 1;
								while (*ptr && isspace(*ptr))
									ptr++;
								if (!*ptr | isspace(*ptr))
									ndos_parrentdir();
								else
								{
									ioprintf("/ unknown command\n");
								}
							}
							else if (!Stricmp(cport->argstring[0],"makefilesystem"))
							{
								LowLevelCommand_makefilesystem();
								nostdcmd = FALSE;
							}
							else if (!Stricmp(cport->argstring[0],"makeroot"))
							{
								LowLevelCommand_makeroot();
								nostdcmd = FALSE;
							}
							else if (!Stricmp(cport->argstring[0],"adddir"))
							{
								LowLevelCommand_adddirectory(cport->argstring[1]);
								nostdcmd = FALSE;
							}
							else if (!Stricmp(cport->argstring[0],"addlink"))
							{
								LowLevelCommand_addlink(cport->argstring[1]);
								nostdcmd = FALSE;
							}
							else if (!Stricmp(cport->argstring[0],"addcommand"))
							{
								LowLevelCommand_addcommand(cport->argstring[1]);
								nostdcmd = FALSE;
							}
							else
							{
								if (ndos_execute(cport->argstring[0]))
								{
									ioprintf("%s: Unknown command\n",cport->argstring[0]);
									sprintf(buffer,"%ssys.mainmenu.mainport",cport->sysdirs->systext);
									ShowText(buffer);LF();
								}
							}
						}
					}
				}
			}
		}
		counter++;
	}
	SaveMainPortConfig(mainportconfig);
}

void c_StartPort()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char buffer[200];
	char Cancel=FALSE;
	ULONG Usernummer,user2;
	char counter=0;
	BPTR mulder;
	ULONG ulong,cmdid;
	BOOL nostdcmd;		// no standart command
	struct PortData *cport2;
	struct PortData2 *nc;

	if (cport->network.online)
	{
		sprintf(buffer,"%ssys.start.net",cport->sysdirs->systext);
		ShowText(buffer);LF();
		ioprintf("NOVIA- Baud Bandit Netware Software.  Version 0.74 alpha \n");
		ioprintf("Copyright (c)1996,1997,1998,1999 by Thorsten Gehler. All rights reserved.\n\n");
	}

	while (!cport->ProgramClose && cport->network.online)
	{
		ChangeWhere(getstr(11,50));
		ChangeScreenStatus("LOCKED");

		cport->PortStatus=PORT_STATUS_LOGIN;

		/*** login Sequence ***/

		ClearMemQuick(buffer,200);
		Writeio("Login: ",-1);
		Getstring(buffer,0,50,0,0);
		Writeio("Password: ",-1);
		Getstring(buffer+100,0,20,0,0);
		ChangeScreenStatus("ONLINE");

		if (cport->network.online && (ulong=SearchUser(buffer)))
		{
			ClearMemQuick(&cport->LocalUser,sizeof(UserData));
			ClearMemQuick(&cport->EditUser,sizeof(UserData));
			if (LoadUser(&cport->LocalUser,ulong))
			{
				ChangeWhere(getstr(11,55));		// Verification
				ioprintf("Current User is :%s (%d)\n",cport->LocalUser.Handle,ulong);
				cport->PortStatus=PORT_STATUS_ONLINE;
				SendMsg(cport->MainPort,cport,0,MSG_PORT_CHANGE,NULL);
			}

		}

		if (cport->network.online && cport->PortStatus==PORT_STATUS_ONLINE)
		{
			ChangeWhere(getstr(11,60));			// Updating database...
			ioprintf("Updating System Database... please wait.\n\n");
			LoadUser(&cport->LocalUser,Usernummer);
			sprintf(buffer,"%ssys.start.new",cport->sysdirs->systext);
			ShowText(buffer);LF();
			sprintf(buffer,"%ssys.welcome.new",cport->sysdirs->systext);
			ShowText(buffer);LF();
		}

		if (ndos_openroot())
			ioprintf("root opened\n");

		while (cport->network.online && cport->PortStatus==PORT_STATUS_ONLINE && !cport->ProgramClose)
		{
			ChangeWhere(getstr(11,65));			// MainMenu

			ioprintf("\nc3%d.%s> c7",cport->clientconfig.IDNumber,cport->currentdirname);
			Getstring(cport->inputbuffer,0,80,BUFFERED,0);
			GetArg(cport->inputbuffer);
			nostdcmd=TRUE;
			switch(cmdid = GetCommandID(cport->argstring[0]))
			{
				case CMD_LIST:
					ioprintf(getstr(11,50));					
					nostdcmd=FALSE;
					break;
				case CMD_EDITCLIENT:
					if (cport->systemservice)
					{
						nostdcmd=FALSE;
					}
					break;
			}
			if (nostdcmd)
			{	
				if (!ndos_changedir(cport->inputbuffer))
				{
					if (StrToLong(cport->inputbuffer,(LONG *)&ulong))
					{
						BOOL founded			= FALSE;
						ItemEntryBlock *ieb	= cport->itementrylist.lh_Head;
						while (!founded && ieb->ln_Succ)
						{
							if (ieb->IDNumber == ulong)
								founded = TRUE;
							else
								ieb = ieb->ln_Succ;
						}
						if (founded)
						{
							if (ieb->ItemType == ITEMTYPE_SUBBOARD) 
							{
								if (!ndos_changedir(ieb->filename))
									founded = FALSE;
							}
							else
							{
								if (ieb->ItemType == ITEMTYPE_MESSAGE)
								ndos_readmessage(ieb);
							}
						}
						if (!founded)
						{
							if (cport->inputbuffer[0] == '*')
							{
								ndos_tstselect(cport->inputbuffer);
							}
							else
							{
								ioprintf("%s: Unknown command\n",cport->argstring[0]);
								sprintf(buffer,"%ssys.mainmenu.mainport",cport->sysdirs->systext);
								ShowText(buffer);LF();
							}
						}
					}
				}
			}
		}
	}
	cport->PortStatus	= PORT_STATUS_WAITCALL;
	SendMsg(cport->MainPort,  cport, 0, MSG_PORT_LOGOFF, NULL);
	SN_HangingUP();
	cport->systemservice=FALSE;
	ioprintf("c7%s c3logged off\n",cport->LocalUser.Handle);
}

BOOL AddClientPort( PortData *cport2)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR mulder;
	struct PortData2 *newclient=MyAllocVec(sizeof(PortData2),MEMF_ANY|MEMF_CLEAR,"main: AddClientPort(), newclient");
	char buffer[80];
	char *txtptr;
	if (newclient==0)return FALSE;
	numberofclients++;
	cport2->UL.UserEntrys=cport->UL.UserEntrys;
	cport2->UL.utable=cport->UL.utable;
	cport2->UL.List_Status=cport->UL.List_Status;
	newclient->port_address=cport2;
	AddTail(client_list,(Node *)newclient);
	sprintf(buffer,"Adding new Port ID:%d",numberofclients);
	SystemMessage(buffer);
	SendGUIMsg(GUIMSG_CLIENT_ADD,cport2);
	return TRUE;
}

struct PortData *NewClientPort()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR mulder;
	struct PortData2 *nc=NULL;
	struct PortData *cport2=NULL;
	char buffer[80];
	char *txtptr;
	if (!(cport2=MyAllocVec(sizeof(PortData),MEMF_ANY|MEMF_CLEAR,"main: NewClientPort(), cport2")))return NULL;
	if (!(nc=MyAllocVec(sizeof(PortData2),MEMF_ANY|MEMF_CLEAR,"main: NewClientPort(), nc")))
	{
		MyFreeVec(cport2,"FREE main: NewClientPort(), cport2");
		return NULL;
	}
	numberofclients++;
	cport2->clientconfig.IDNumber=numberofclients;
	cport2->UL.UserEntrys=cport->UL.UserEntrys;
	cport2->UL.utable=cport->UL.utable;
	cport2->UL.List_Status=cport->UL.List_Status;
	nc->port_address=cport2;
	AddTail(client_list,(Node *)nc);
	sprintf(buffer,"Adding new Port ID:%d",numberofclients);
	SystemMessage(buffer);

	sprintf(buffer,"%ssys.clientconfig",cport->sysdirs->sysdata);

	mulder=Open(buffer,MODE_READWRITE);
	Seek(mulder,sizeof(ClientConfig)*cport2->clientconfig.IDNumber-1,OFFSET_BEGINNING);
	FWrite(mulder,&cport2->clientconfig,sizeof(ClientConfig),1);
	Close(mulder);

	SaveClientList();
	SendGUIMsg(GUIMSG_CLIENT_ADD,cport2);
	return cport2;
}


/*
BOOL ClearClientPort( PortData *cport2)
	cport2->PortStatus=PORT_STATUS_
*/
BOOL CloseClientPort( LONG PortID)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct PortData2 *nc=(PortData2 *)client_list->lh_Head;
	struct PortData *cport2;
	BOOL removed=FALSE;
	char buffer[200];
	ULONG counter;
	if (PortID>=0)
	{
		while (nc->port_node.ln_Succ)
		{
			if (nc->port_address->clientconfig.IDNumber==PortID)
			{
				if (nc->port_address->PortStatus>0)
				{
					SendMsg(nc->port_address->LocalPort,nc->port_address,0,MSG_PORT_CLOSE,0);
				}
				else
				{
					nc=(PortData2 *)nc->port_node.ln_Succ;
					ioprintf("c3Port c7%d c3is already closed.\n",nc->port_address->clientconfig.IDNumber);
				}
				return removed;
			}
			nc=(PortData2 *)nc->port_node.ln_Succ;
		}
		ioprintf("c7Port c3%d c7is closed now.\n",removed);
	}
	return removed;
}

/*
BOOL RemoveClientPort( PortData2 *nc)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	PortData *cport2;
	if (nc)
	{
		Remove((Node *)nc);
		cport2=nc->port_address;
		MyFreeVec(cport2,"");
		MyFreeVec(nc,"");
		SystemMessage("Port removed.");
		numberofclients--;
	}
	else
	{
		SystemMessage("Can't remove client, wrong NC-Pointer.");
		return FALSE;
	}
}
*/

ULONG SearchClientORUser( char *name)
{
/*	char buffer[100];
	UserNode *start;
	ULONG counter;
 	BPTR mulder;
	long index;
	char Quit=FALSE;
	StrToLong(username,(long *)&counter);
	if (counter>0)
	{
		for (start=cport->UL.un_first;(start!=0 && Quit==FALSE);start=start->us_Succ)
			if (start->IDNumber==counter)Quit=TRUE;
		if (Quit==TRUE)
		{
			sprintf(buffer,"Is %s correct ?",start->Handle);
			LF();Writeio(buffer,-1);
			if (Yesno(TRUE,0)==TRUE)return counter;
		}
	}
	else
	{
		start=cport->UL.un_first;
		for (counter=0;counter<cport->UL.UserEntrys;counter++)
		{
			if (!stricmp(start-Handle,username))return start->IDNumber;
			start=start->us_Succ;
		}
	}*/
	return 1;
}


void FreeGUIMsg( GUIMsg *msg)
{
	MyFreeVec(msg,"FREE main: FreeGUIMsg(), msg");
}

void ReturnGUIMsg(GUIMsg *msg)
{
	msg->cmdid=GUIMSG_REPLY;
	if (GUIPort)ReplyMsg((Message *)msg);
}

BOOL SendGUIMsg( LONG msgid, APTR data)
{
	struct GUIMsg *msg;
	if (GUIPort)
	{
		msg=MyAllocVec(sizeof(GUIMsg),MEMF_ANY|MEMF_CLEAR,"main: SendGUIMsg(), msg");
		if (!(msg))return FALSE;
		msg->data=data;
		msg->msg.mn_Length=sizeof(GUIMsg);
		msg->msg.mn_ReplyPort=MainGUIPort;
		msg->cmdid=msgid;
		PutMsg(GUIPort,(Message *)msg);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void LoadClientList()
{
	struct PortData *cport=MainTask->tc_UserData;
	BPTR mulder;
	char buffer[100];
	struct PortData2 *nc;
	struct PortData *cport2;
	BOOL quit=FALSE;
	ULONG result;
	sprintf(buffer,"%ssys.clientconfig",mainportconfig->sysdirs.sysdata);
	mulder=Open(buffer,MODE_OLDFILE);
	if (!mulder)return;
	SetIoErr(0);
	while(!quit)
	{
		if (nc=(PortData2 *)MyAllocVec(sizeof(PortData2),MEMF_ANY|MEMF_CLEAR,""))
		{
			if (cport2=(PortData *)MyAllocVec(sizeof(PortData),MEMF_ANY|MEMF_CLEAR,""))
			{
				result=FRead(mulder,&cport2->clientconfig,sizeof(ClientConfig),1);
				if (!result)
				{
					quit=TRUE;
					MyFreeVec(cport2,"");
					MyFreeVec(nc,"");
				}
				else
				{
					nc->port_address=cport2;
					nc->port_address->PortStatus=0;
					AddTail(client_list,(Node *)nc);
					numberofclients++;
					nc->port_address->clientconfig.IDNumber=numberofclients;
					nc->port_address->UL.UserEntrys=cport->UL.UserEntrys;
					nc->port_address->UL.utable=cport->UL.utable;
					nc->port_address->UL.List_Status=cport->UL.List_Status;
					nc->port_address->sysdirs=&mainportconfig->sysdirs;
					nc->port_address->strgroup=cport->strgroup;
					nc->port_address->strgroups=cport->strgroups;
					if (nc->port_address->clientconfig.LoadPortatstart)
					{
						ioprintf("setting up client id: %d",nc->port_address->clientconfig.IDNumber);
						nc->port_address->PortStatus=PORT_STATUS_WAITLOAD;
							CreateClient(nc->port_address);
						CR();ioprintf("[70Cc2donec7\n");
					}
				}
			}
			else
			{
				MyFreeVec(nc,"");
				quit=TRUE;
			}
		}
		else {quit=TRUE;}
	}
	Close(mulder);
	SendGUIMsg(GUIMSG_CLIENTLIST_REFRESH,0);
}

void SaveClientList()
{
	BPTR mulder;
	char buffer[100];
	struct PortData2 *nc;
	BOOL quit=FALSE;
	ULONG counter=0;
	sprintf(buffer,"%ssys.clientconfig",mainportconfig->sysdirs.sysdata);
	mulder=Open(buffer,MODE_NEWFILE);
	if (!mulder)return;
	nc=(PortData2 *)client_list->lh_Head;
	while (nc->port_node.ln_Succ!=0 && counter<10)
	{
		FWrite(mulder,&nc->port_address->clientconfig,sizeof(ClientConfig),1);
		nc=(PortData2 *)nc->port_node.ln_Succ;
		counter++;
	}
	Close(mulder);
}

APTR MyAllocVec( ULONG size,  ULONG flags,  char *text)
{
/*	APTR address=AllocVec(size,flags);
	MyMemEntry *entry;
	char txtout[80];
	BPTR mulder=Open("boot:logs/log",MODE_OLDFILE);
	if (address)
	{
		entry=AllocVec(sizeof(MyMemEntry),MEMF_ANY);
		entry->address=address;
		alloc_mem_list->entrys++;
		AddTail((List *)alloc_mem_list,(Node *)entry);
		sprintf(entry->text,"FREE  MAIN (OK): %p %s - Entry: %d\n",address,text,alloc_mem_list->entrys);
		if (mulder)
		{
			sprintf(txtout,"ALLOC MAIN (OK): %p %s\n",address,text);
			Seek(mulder,0,OFFSET_END);
			FPuts(mulder,txtout);
		}
	}
	else
	{
		sprintf(txtout,"ALLOC MAIN (FAILAT): %p %s\n",address,text);
		if (mulder)
		{
			Seek(mulder,0,OFFSET_END);
			FPuts(mulder,txtout);
		}
	}
	if (mulder)Close(mulder);
	return address;*/
	return AllocVec(size,flags);
}

void MyFreeVec( APTR address,  char *text)
{
/*	MyMemEntry *entry=(MyMemEntry *)alloc_mem_list->lh_Head;
	BOOL quit=FALSE;
	char txtout[80];
	BPTR mulder=Open("boot:logs/log",MODE_OLDFILE);
	while (entry && !quit)
	{
		if(entry->address==address)
		{	
			if (mulder)
			{
				sprintf(txtout,"FREE  MAIN (OK): %p %s - entry: %d\n",address,text,alloc_mem_list->entrys);
				Seek(mulder,0,OFFSET_END);
				FPuts(mulder,txtout);
			}
			quit=TRUE;
			Remove((Node *)entry);
			FreeVec(address);
			FreeVec(entry);
			alloc_mem_list->entrys--;
		}
		entry=(MyMemEntry *)entry->ln_Succ;
	}
	if (!quit)
	{
		sprintf(txtout,"FREE  MAIN (FAILAT): %p %s - entry: %d\n",address,text,alloc_mem_list->entrys);
		if (mulder)
		{
			Seek(mulder,0,OFFSET_END);
			FPuts(mulder,txtout);
		}
	}
 
	if (mulder)Close(mulder);*/
	FreeVec(address);
}

void SaveClientConfig( ClientConfig *cconfig)
{
	BPTR mulder;
	char buffer[100];
	sprintf(buffer,"%ssys.clientconfig",mainportconfig->sysdirs.sysdata);
	mulder=Open(buffer,MODE_READWRITE);
	if (!mulder)return;
	Seek(mulder,(cconfig->IDNumber-1)*sizeof(ClientConfig),OFFSET_BEGINNING);
	Write(mulder,cconfig,sizeof(ClientConfig));
	Close(mulder);
}

void SaveMainPortConfig( MainPortConfig *pointer)
{
	BPTR mulder=Open("envarc:novia_mainportconfig",MODE_NEWFILE);
	if (!mulder)return;
	Write(mulder,pointer,sizeof(MainPortConfig));
	Close(mulder);
}

struct MainPortConfig *LoadMainPortConfig( MainPortConfig *pointer)
{
	struct MainPortConfig *result;
	BPTR mulder;
	if ((mulder = Open("envarc:novia_mainportconfig",MODE_OLDFILE)))
	{
		if (Read(mulder,pointer,sizeof(MainPortConfig)) == sizeof(MainPortConfig))
		{
			BPTR fh;			
			pointer->numberofmci		= 0;
			pointer->mcicmd			= 0;
			NewList((List *)&pointer->ClientList);
			pointer->rexxtempcounter= 0;
			pointer->guiloaded		= 0;
			NewList((List *)&pointer->LocalMailServerList);
			NewList((List *)&pointer->POP3ServerList);
			NewList((List *)&pointer->EventList);
			pointer->rootdir			= 0;

			if ((fh = Open("novia:sysdata/counter/ipcounter", MODE_OLDFILE)))
			{
				Read(fh, &pointer->MessageIPCounter64, 8);
				Close(fh);
			}

			result = pointer;
		}
		Close(mulder);
	}
	else
	{
		printf("can't open ENVARC:novia_mainportconfig.\n");
	}
	return(result);
}


void LoadCommandList(Directory *cmddir)
{
	struct PortData		*cport = (PortData *)MainTask->tc_UserData;
	char						buffer[100];
	BPTR						fh;
	struct CommandEntry	*commandentry;
	char						quit = FALSE;
	sprintf(buffer,"novia:bin/.files");

	Writeio("c7z0configure low level commandlist...", -1);
	NewList((List *)&cmddir->entrylist);
	/* Add Kernel Low Level Commands */

	if (cport->cmddir->iib.fullpath)
		FreeVec(cport->cmddir->iib.fullpath);
	if ((cport->cmddir->iib.fullpath = AllocVec(strlen("novia:bin/") + 1, MEMF_ANY | MEMF_CLEAR)))
		strcpy(cport->cmddir->iib.fullpath, "novia:bin/");
	if ((fh = Open(buffer,MODE_OLDFILE)))
	{
		while (!quit)
		{
			if ((commandentry = AllocVec(sizeof(CommandEntry), MEMF_ANY|MEMF_CLEAR)))
			{
				if (Read(fh, commandentry, sizeof(CommandEntry)) == sizeof(CommandEntry))
				{
					if (!commandentry->iib.Deleted)
					{
						AddTail((List *)&cmddir->entrylist, (Node *)commandentry);
						cmddir->ItemEntrys++;
					}
					else
						FreeVec(commandentry);
				}
				else
				{
					CR();ioprintf("[70Cc2donec7\n");
					quit = TRUE;
				}
			}
			else
			{
				CR();ioprintf("[70Cc1failatc7\n");
				printf("can't open: %s\n",buffer);
				quit = TRUE;
			}
		}
		Close(fh);
	}
	else
	{
		CR();ioprintf("[70Cc1failatc7\n");
		printf("can't open: %s\n",buffer);
	}
}


void LoadMCICodes(ULONG *numberofmci, struct mcicode *mcicmd)
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	char oldcolor=cport->color;
	char oldbcolor=cport->bcolor;
	BPTR mulder;
	char buffer[100];
	sprintf(buffer,"%ssys.mcioffsettable",mainportconfig->sysdirs.sysdata);
	mulder=Open(buffer,MODE_OLDFILE);
	Writeio("c7z0creating MCI-Offset table... ",-1);
	if (mulder==NULL)
	{
		ioprintf("can't open MCI Offsettable");
		CR();ioprintf("[70Cc1failatc7\n");
		return;
	}
	Read(mulder,numberofmci,4);
	ioprintf("%d MCI's found",*numberofmci);
	if (*numberofmci>0)
	{
		if (!(mcicmd=MyAllocVec(sizeof(mcicode)*(*numberofmci),MEMF_ANY,"LoadMCICodes: mcicode")))
			CloseIt(20,"Can't alloc mem for MCI Offsettable");
		Read(mulder,mcicmd,(*numberofmci)*sizeof(mcicode));
	}
	else
		CloseIt(20,"Can't load MCI Offsettable");
	Close(mulder);
	CR();ioprintf("[70Cc2donec7\n");
	SetColor(oldcolor);
	SetBColor(oldbcolor);
}

long GetCommandID(char *string)
{
	struct PortData	*cport	= (PortData *)FindTask(NULL)->tc_UserData;
	ULONG					counter;
	long					result	= NULL;
	char					*ptr		= string;
	CommandEntry		*commandentry;
	while (*ptr)
	{
		*ptr = toupper(*ptr);
		ptr++;
	}
	commandentry = (CommandEntry *)cport->cmddir->entrylist.mlh_Head;
	while (!result && commandentry->iib.ln_Succ)
	{
		if (!strcmp(commandentry->iib.Title, string))
		{
			result = commandentry->CommandID;
		}
		else
			commandentry = (CommandEntry *)commandentry->iib.ln_Succ;
	}
	return result;
}

BOOL LoadMemberList(char *filename, List *ulist)
{
	BPTR mulder=Open(filename,MODE_OLDFILE);
	if (mulder)
	{
		char buffer[80];
		ULONG userid;
		LONG DOSError;
		char counter;
		struct Member *member;
		NewList(ulist);
		while (FGets(mulder,buffer,80))
		{
			for (counter=0; (counter<80 && buffer[counter]!=10 && buffer[counter]!=13);counter++);
			if (buffer[counter]==10|buffer[counter]==13)buffer[counter]=0;
			if ((userid=FindUser(buffer)))
			{
				if (member=AllocVec(sizeof(Member),MEMF_ANY))
				{
					member->UserID=userid;
					AddTail(ulist,(Node *)member);
				}
			}
		}
		if (DOSError=IoErr())
			PrintDosError(DOSError);
		Close(mulder);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL SaveMemberList(char *filename, List *ulist)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR mulder=Open(filename,MODE_NEWFILE);
	if (mulder)
	{
		struct Member *member=(Member *)ulist->lh_Head;
		while(member->ln_Succ)
		{
			if (member->UserID>0 && member->UserID<=cport->UL.UserEntrys)
			{
				FPuts(mulder,cport->UL.utable[member->UserID].Handle);
				FPutC(mulder,10);
			}
			member=member->ln_Succ;
		}
		Close(mulder);
		return TRUE;
	}
	else
		return FALSE;
}

void RepairUserList( UserList *ulist)
{
	struct PortData *cport = (PortData *)FindTask(NULL)->tc_UserData;
	UserNode start;
	ULONG counter=1;
	BPTR mulder;
	char buffer[100];

	sprintf(buffer,"%ssys.userdata.a",mainportconfig->sysdirs.sysdata);

	if (mulder=Open(buffer,MODE_NEWFILE))
	{
		Write(mulder,&counter,4);
		while (LoadUser(&cport->EditUser,counter))
		{
			memset(&start,0,sizeof(UserNode));
			start.IDNumber=cport->EditUser.IDNumber;
			start.IPNumber=cport->EditUser.IPNumber;
			CopyMem(cport->EditUser.Handle,start.Handle,20);
			start.Status=1;
			Write(mulder,&start,sizeof(UserNode));
			counter++;
		}
		counter--;
		Seek(mulder,0,OFFSET_BEGINNING);
		Write(mulder,&counter,4);
		Close(mulder);
	}
}

void LoadStringTable()
{
	#define MAX_ENTRYS 1000
	#define MIN_ENTRYS 50

	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	BPTR mulder;
	char readbuffer[256];
	UWORD stringid=0;
	ULONG strlength;
	ULONG ulong;
	ULONG linecounter=1;
	struct STR_Group *grouppt=NULL;
	char *pt;
	cport->strgroups=256;

	if (cport->strgroup=AllocVec(sizeof(STR_Group)*256,MEMF_ANY|MEMF_CLEAR))
	{
		sprintf(readbuffer,"%ssys.strings",cport->sysdirs->sysdata);
		if (mulder=Open(readbuffer,MODE_OLDFILE))
		{
			BOOL quit=FALSE;
			while (!quit && FGets(mulder,readbuffer,255))
			{
				strlength=strlen(readbuffer);
				if (readbuffer[strlength-1]==10)readbuffer[strlength-1]=0;
				if ((strlength>5) && (!strncmp(readbuffer,"GROUP",5)))
				{
					pt=readbuffer+5;
					while (*pt==32)pt++;
					StrToLong(pt,(LONG *)&ulong);
					if (ulong>cport->strgroups)
					{
						ioprintf("string table error in line: %d. invalid GROUPID: %d\n->%s\n",linecounter,ulong,readbuffer);
						grouppt=NULL;
					}
					else
						grouppt=&cport->strgroup[ulong];
				}
				else
				{
					if (grouppt && (StrToLong(readbuffer,(LONG *)&ulong)>-1))
					{
						stringid=(UWORD)ulong;
						if (stringid>MAX_ENTRYS)
							ioprintf("string table error in line: %d. invalid STRINGID: %d\n->%s\n",linecounter,stringid,readbuffer);
						else
						{
							if (stringid>=grouppt->entrys)
							{
								if (grouppt->entrys)
								{
									char **newtable;
									if (newtable=AllocVec((stringid+MIN_ENTRYS)*4,MEMF_ANY|MEMF_CLEAR))
									{
										for (ulong=0;ulong<grouppt->entrys;ulong++)
										{
											newtable[ulong]=grouppt->table[ulong];
										}
										FreeVec(grouppt->table);
										grouppt->table=newtable;
										grouppt->entrys=stringid+MIN_ENTRYS;
									}
								}
								else
								{
									if (grouppt->table=AllocVec(MIN_ENTRYS*4,MEMF_ANY|MEMF_CLEAR))
										grouppt->entrys=MIN_ENTRYS;
								}
							}
							if (stringid<grouppt->entrys)
							{
								pt=readbuffer;
								while (*pt!=':' && *pt!=0)pt++;
								if (*pt!=0)
								{
									pt++;
									strlength=strlen(pt);
									if (grouppt->table[stringid])
										ioprintf("string table error in line: %d. String redefined ID: %d\n->%s\n",linecounter,stringid,readbuffer);
									else
										if (grouppt->table[stringid]=AllocVec(strlength+1,MEMF_ANY|MEMF_CLEAR))
										{
											CopyMem(pt,grouppt->table[stringid],strlength);
											grouppt->maxid++;
										}
								}
							}
						}
					}
				}
				linecounter++;
			}
			Close(mulder);
		}
		else
		{
			ioprintf("can't open %s\n.",readbuffer);
			FreeVec(cport->strgroup);
			cport->strgroups=0;
			cport->strgroup=NULL;
		}
	}
}

void RemoveStringList()
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	struct STR_Group *grouppt=cport->strgroup;
	UWORD counter,counter2;
	if (grouppt && cport->strgroups)
	{
		for (counter=0; counter<cport->strgroups; counter++)
		{
			if (grouppt->table)
			{
				for (counter2=0; counter2<grouppt->entrys; counter2++)
				{
					if (grouppt->table[counter2])
						FreeVec(grouppt->table[counter2]);
				}
				FreeVec(grouppt->table);
			}
			grouppt++;
		}
		FreeVec(cport->strgroup);
		cport->strgroup=0;
		cport->strgroups=0;
	}
}


struct PortData *c_init()
{
	ULONG returncode=FALSE;
	struct PortData *mport=(PortData *)MainTask->tc_UserData;
	struct PortData *cport=FindTask(NULL)->tc_UserData;
	char buffer[200];
	if (cport->clientconfig.IDNumber)
	{
		sprintf(cport->RexxPortName,"NOVIAREXX%d",cport->clientconfig.IDNumber);
		sprintf(cport->TaskName,"novia_client%d",cport->clientconfig.IDNumber);

		if (cport->currentdir)
			nfs_support_closeroot(cport);

		cport->ProgramClose=FALSE;
		cport->ConPort=CreateMsgPort();
		cport->MainPort=mport->MainPort;
		cport->LocalPort=CreateMsgPort();
		cport->rexxport=CreateMsgPort();
		cport->rtport=CreateMsgPort();
		cport->task=FindTask(NULL);
		cport->nfs_port = CreateMsgPort();

		if (!cport->ConPort | !cport->LocalPort | !cport->rexxport | !cport->rtport | !cport->nfs_port)
		{
			printf("CLIENT: can't create LocalPort.\n");
			returncode=TRUE;
		}

		if (!returncode)
		{
			if (!(cport->inputbuffer = AllocVec(256,MEMF_ANY|MEMF_CLEAR)))
				returncode = TRUE;
		}

		if (!returncode)
		{
			if (cport->ConPort)
			{
				cport->ConRead=CreateIORequest(cport->ConPort,sizeof(IOStdReq));
				cport->ConWrite=CreateIORequest(cport->ConPort,sizeof(IOStdReq));
				if ((!cport->ConRead)|(!cport->ConWrite))
				{
					returncode=TRUE;
					printf("CLIENT: Can't create IORequest-structure for console.device\n");
				}
			}
			else
			{
				printf("CLIENT: Can't create MSG-Port for console.device\n");
				returncode=TRUE;
			}
		}

		if (!returncode)
		{
			if ((cport->TimePort=CreateMsgPort()))
			{
				if (cport->TimerReq=CreateIORequest(cport->TimePort,sizeof(timerequest)))
					if (OpenDevice("timer.device",UNIT_VBLANK,(IORequest *)cport->TimerReq,0))
					{
						printf("can't open timer.device.\n");
						returncode=20;
					}
					else
					{
						TimerBase = (struct Library *)cport->TimerReq->tr_node.io_Device;
						if ((!cport->TimerReq)|(!TimerBase))
						{
							printf("can't open timer.device.\n");
							returncode=TRUE;
						}
					}
			}
			else
			{
				printf("Can't create MSG-Port for timer.device\n");
				returncode=TRUE;
			}
		}
		if (!returncode)
		{
			cport->Titel=Titel;
//			strcpy(cport->actitel,Titel);
			if (!(cport->iobuffer=AllocVec(10000,MEMF_ANY|MEMF_CLEAR)))
			{
				printf("CLIENT: can't alloc mem for iobuffer.\n");
				returncode=TRUE;
			}	
		}

		NewList((List *)&cport->iobuffer_list);
		NewList((List *)&cport->his_list);
		NewList((List *)&cport->select_list);
		NewList((List *)&cport->downloadlist);
		NewList((List *)&cport->rexxlist);
		NewList((List *)&cport->itementrylist);
		cport->connect_seq=TRUE;

		if (cport->rexxport)
		{
			cport->rexxport->mp_Node.ln_Pri=0;
			cport->rexxport->mp_Node.ln_Name=cport->RexxPortName;
			AddPort(cport->rexxport);
		}
		else
		{
			printf("can't create rexxport.\n");
			returncode=TRUE;
		}

		if (!returncode)
		{
			cport->mymenu=mymenu;
			cport->Supervisor=TRUE;
			cport->NewUser=FALSE;
			cport->Dumped=FALSE;
			cport->HideAll=FALSE;
			cport->MuffAll=FALSE;
			cport->MonitorAll=FALSE;
			cport->UL=mport->UL;
			cport->PortStatus=PORT_STATUS_ONLINE;

			if (cport->clientconfig.OpenScreenatstart)
				Monitor_open();

			ShowText("work:novia/systext/sys.novia.text");
#ifdef DEBUG
			printf("next\n");
#endif
			cport->strgroup=mport->strgroup;
			cport->strgroups=mport->strgroups;
			cport->cmddir=mport->cmddir;
			cport->mcicmd=mport->mcicmd;
			cport->numberofmci=mport->numberofmci;
		}
		else
		{
			c_FreeClientPort();
		}
		cport->task=FindTask(NULL);
	}
	if (!returncode)
	{
		SN_Open();
		return cport;
	}
	else
		return NULL;
}

struct PortData *CreateClient(PortData *cport)
{
	char error=FALSE;
	struct PortData *mport=(PortData *)MainTask->tc_UserData;
	struct Task *ClientTask=NULL;
	char buffer[200];
	struct TagItem ClientProcTagList[] =
	{
		{	NP_Entry,		(ULONG)c_main},
		{	NP_Name,			(ULONG)cport->TaskName},
		{	NP_StackSize,	20000},
		{	TAG_DONE,		TAG_DONE},
	};
	sprintf(cport->TaskName,"novia_client%d",cport->clientconfig.IDNumber);
	Forbid();
	if ((ClientTask=(struct Task *)CreateNewProc(ClientProcTagList)))
		ClientTask->tc_UserData=cport;
	Permit();
	return cport;
}


void c_FreeClientPort()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		SN_Close();
		if (cport->win)
			ioprintf("Shut down\n");
		Monitor_close();
		if (cport->rexxport)
		{
			RemPort(cport->rexxport);
			DeleteMsgPort(cport->rexxport);
			cport->rexxport=NULL;
		}
		NewList((List *)&cport->iobuffer_list);
		NewList((List *)&cport->his_list);
		NewList((List *)&cport->select_list);
		NewList((List *)&cport->downloadlist);
		NewList((List *)&cport->rexxlist);
		NewList((List *)&cport->itementrylist);
		if (cport->inputbuffer)
		{
			FreeVec(cport->inputbuffer);
			cport->inputbuffer = 0;
		}
		if (cport->iobuffer)
		{
			FreeVec(cport->iobuffer);
			cport->iobuffer=NULL;
		}
		if (cport->ConPort)
		{
			DeleteMsgPort(cport->ConPort);
			cport->ConPort=NULL;
		}
		if (cport->ConRead)
		{
			DeleteIORequest(cport->ConRead);
			cport->ConRead=NULL;
		}
		if (cport->ConWrite)
		{
			DeleteIORequest(cport->ConWrite);
			cport->ConWrite=NULL;
		}
		if (cport->LocalPort)
		{
			DeleteMsgPort(cport->LocalPort);
			cport->LocalPort=NULL;
		}
		if (cport->TimerReq)
		{
			if (cport->TimerReq->tr_node.io_Device)
				CloseDevice((IORequest *)cport->TimerReq);
			DeleteIORequest((IORequest *)cport->TimerReq);
		}

		if (cport->rtport)
			DeleteMsgPort(cport->rtport);

		if (cport->TimePort)
			DeleteMsgPort(cport->TimePort);

		if (cport->nfs_port)
		{
			DeleteMsgPort(cport->nfs_port);
			cport->nfs_port = NULL;
		}
		FindTask(NULL)->tc_UserData=NULL;
		cport->PortStatus=NULL;
		cport->task=NULL;
	}
#ifdef DEBUG
	printf("port closed: %d\n.",cport->clientconfig.IDNumber);
#endif
//	Exit(0);
}


void StartGUI()
{
	char error=FALSE;
	struct PortData *mport=(PortData *)MainTask->tc_UserData;
	char buffer[200];
	struct TagItem GUIProcTagList[] =
	{
		{	NP_Entry,		(ULONG)gui_main},
		{	NP_Name,			(ULONG)"novia_gui"},
		{	NP_StackSize,	20000},
		{	TAG_DONE,		TAG_DONE},
	};
	guifrontend.mport=mport;
	if ((GUITask=(Task *)CreateNewProc(GUIProcTagList)))
	{
		GUITask->tc_UserData=&guifrontend;
		guifrontend.GUIOpen=TRUE;
	}
}



void nrc_start()
{
	struct TagItem NRCProcTagList[] =
	{
		{	NP_Entry,		(ULONG)nrc_main},
		{	NP_Name,			(ULONG)"novia_nrc_server"},
		{	NP_StackSize,	20000},
		{	TAG_DONE,		TAG_DONE},
	};
	CreateNewProc(NRCProcTagList);
}

void nfs_start()
{
	struct TagItem NFSProcTagList[] =
	{
		{	NP_Entry,		(ULONG)nfs_main},
		{	NP_Name,			(ULONG)"novia_nfs_server"},
		{	NP_StackSize,	20000},
		{	TAG_DONE,		TAG_DONE},
	};
	ioprintf("starting Novia File Server");
	CreateNewProc(NFSProcTagList);
	while (!nfsserver)
		Delay(2);
	CR(); ioprintf("[70Cc2donec7\n");
}


void LoadEvents()
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	char *buffer;
	BPTR mulder;
	if ((buffer=AllocVec(1024,MEMF_ANY|MEMF_CLEAR)))
	{
		sprintf(buffer,"%ssys.events",cport->sysdirs->sysdata);
		if ((mulder=Open(buffer,MODE_OLDFILE)))
		{
			struct Event *event;
			char quit=FALSE;
			ULONG strlength;
			char start=FALSE;
			ULONG	linecounter=1;

			while (!quit && FGets(mulder,buffer,1023)) // Nicht 1024, wegen eines Bugs in der dos.library V37
			{
				strlength=strlen(buffer);
				if (searchstr(buffer,"STARTEVENT"))
				{
					if (!start)
					{
						start=TRUE;
						if ((event=AllocVec(sizeof(Event),MEMF_ANY|MEMF_CLEAR)))
						{
							event->Multiplicator= MULTIPLICATOR_MINS;
						}
						else
						{
							quit=TRUE;
						}
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT.",linecounter);
					}
				}
				if (searchstr(buffer,"ENDEVENT"))
				{
					if (start)
					{
						event->Status=TRUE;
						AddTail((List *)&mainportconfig->EventList,(Node *)event);
						start=FALSE;
					}
					else
					{
						printf("eventlist error in line %d, END.",linecounter);
					}
				}
				if (searchstr(buffer,"INTERVALTIME="))
				{
					if (start)
					{
						ULONG ulong;
						char *pos=&buffer[12];
						while ((*pos<'0' | *pos>'9') && *pos)
							pos++;
						StrToLong(pos,(LONG *)&ulong);
						event->IntervalTime=ulong;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"INTERVALMULTIPLICATOR="))
				{
					if (start)
					{
						if (searchstr(buffer,"SECS"))
							event->Multiplicator=MULTIPLICATOR_SECS;
						if (searchstr(buffer,"MINS"))
							event->Multiplicator=MULTIPLICATOR_MINS;
						if (searchstr(buffer,"HOURS"))
							event->Multiplicator=MULTIPLICATOR_HOURS;
						if (searchstr(buffer,"DAYS"))
							event->Multiplicator=MULTIPLICATOR_DAYS;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"EVENTNAME="))
				{
					if (start)
					{
						mystrcpy(event->Name,buffer,20)
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"BEGINDATE="))
				{
					if (start)
					{
						if (event->EndDate.datecode==0)
						{
							ULONG ulong;
							char *pos=&buffer[4];
							StringToDate(pos,&event->BeginDate,0);
						}
						else
							printf("eventlist error in line %d, BEGINDATE alread set.\n",linecounter);					
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"ENDDATE="))
				{
					if (start)
					{
						if (event->EndDate.datecode==0)
						{
							ULONG ulong;
							char *pos=&buffer[4];
							StringToDate(pos,&event->EndDate,0);
						}
						else
							printf("eventlist error in line %d, ENDDATE alread set.\n",linecounter);
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"BEGINTIME="))
				{
					if (start)
					{
						struct Date mydate;
						ULONG ulong;
						char *pos=&buffer[10];
						StringToDate(pos,&mydate,0);
						event->BeginDate.hours=mydate.hours;
						event->BeginDate.mins=mydate.mins;
						event->BeginDate.secs=mydate.secs;
						CalcDatecode(&event->BeginDate);
						DateToString(buffer,&mydate,0);
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"ENDTIME="))
				{
					if (start)
					{
						struct Date mydate;
						ULONG ulong;
						char *pos=&buffer[8];
						StringToDate(pos,&mydate,0);
						event->EndDate.hours=mydate.hours;
						event->EndDate.mins=mydate.mins;
						event->EndDate.secs=mydate.secs;
						CalcDatecode(&event->EndDate);
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"DATETYPE="))
				{
					if (start)
					{
						if (searchstr(buffer,"DATE"))
							event->DateType=0;
						if (searchstr(buffer,"SPACE"))
							event->DateType=1;
						if (searchstr(buffer,"MONTH"))
							event->DateType=2;
						if (searchstr(buffer,"DIALY"))
							event->DateType=3;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"USEINTERVAL="))
				{
					if (start)
					{
						if (searchstr(buffer,"TRUE"))
							event->UseInterval=TRUE;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"HOLIDAYS="))
				{
					if (start)
					{
						if (searchstr(buffer,"ONLY"))
							event->Holiday=1;
						else
							if (searchstr(buffer,"DONOTHING"))
								event->Holiday=2;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"WEEKDAYS="))
				{
					if (start)
					{
						if (searchstr(buffer,"MON"))
							event->weekday[0]=TRUE;
						if (searchstr(buffer,"THU"))
							event->weekday[1]=TRUE;
						if (searchstr(buffer,"WEN"))
							event->weekday[2]=TRUE;
						if (searchstr(buffer,"TUR"))
							event->weekday[3]=TRUE;
						if (searchstr(buffer,"FRI"))
							event->weekday[4]=TRUE;
						if (searchstr(buffer,"SAT"))
							event->weekday[5]=TRUE;
						if (searchstr(buffer,"SUN"))
							event->weekday[6]=TRUE;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"USEWEEKDAYS="))
				{
					if (start)
					{
						if (searchstr(buffer,"TRUE"))
							event->UseWeekday=TRUE;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				if (searchstr(buffer,"MONTH"))
				{
					if (start)
					{
						if (searchstr(buffer,"JAN"))
							event->month[0]=TRUE;
						if (searchstr(buffer,"FEB"))
							event->month[1]=TRUE;
						if (searchstr(buffer,"MRZ"))
							event->month[2]=TRUE;
						if (searchstr(buffer,"APR"))
							event->month[3]=TRUE;
						if (searchstr(buffer,"MAY"))
							event->month[4]=TRUE;
						if (searchstr(buffer,"JUN"))
							event->month[5]=TRUE;
						if (searchstr(buffer,"JUL"))
							event->month[6]=TRUE;
						if (searchstr(buffer,"AUG"))
							event->month[7]=TRUE;
						if (searchstr(buffer,"SEP"))
							event->month[8]=TRUE;
						if (searchstr(buffer,"OCT"))
							event->month[9]=TRUE;
						if (searchstr(buffer,"NOV"))
							event->month[10]=TRUE;
						if (searchstr(buffer,"DEC"))
							event->month[11]=TRUE;
					}
					else
					{
						printf("eventlist error in line %d, STARTEVENT fails.",linecounter);
					}
				}
				linecounter++;
			}
			Close(mulder);
		}
		else
		{
			printf("can't load eventlist. unable to open %s.\n",buffer);
		}
	}
	else
	{
		printf("can't open ENVARC:novia_mainportconfig.\n");
	}
}
	


void SaveEvents()
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	char buffer[200];
	BPTR mulder;
	sprintf(buffer,"%ssys.events",cport->sysdirs->sysdata);
	if ((mulder=Open(buffer,MODE_NEWFILE)))
	{
		struct Event *event=(Event *)mainportconfig->EventList.mlh_Head;
		while (event->ln_Succ)
		{
			sprintf(buffer,"STARTEVENT\n");
			FPuts(mulder,buffer);
			sprintf(buffer,"EVENTNAME=%s\n",event->Name);
			FPuts(mulder,buffer);

			if (event->BeginDate.year|event->BeginDate.month|event->BeginDate.day)
			{
				struct Date *d=&event->BeginDate;
				DateToString(&buffer[100],d,DATEFLAG_USEDATE);
				sprintf(buffer,"BEGINDATE=%s\n",&buffer[100]);
				FPuts(mulder,buffer);
			}

			if (event->EndDate.year|event->EndDate.month|event->EndDate.day)
			{
				struct Date *d=&event->EndDate;
				DateToString(&buffer[100],d,DATEFLAG_USEDATE);
				sprintf(buffer,"ENDDATE=%s\n",&buffer[100]);
				FPuts(mulder,buffer);
			}

			if (event->IntervalTime)
			{
				sprintf(buffer,"INTERVALTIME=%d\n",event->IntervalTime);
				FPuts(mulder,buffer);
			}

			switch (event->Multiplicator)
			{
				case MULTIPLICATOR_SECS:
					FPuts(mulder,"INTERVALMULTIPLICATOR=SECS\n");
					break;
				case MULTIPLICATOR_MINS:
					FPuts(mulder,"INTERVALMULTIPLICATOR=MINS\n");
					break;
				case MULTIPLICATOR_HOURS:
					FPuts(mulder,"INTERVALMULTIPLICATOR=HOURS\n");
					break;
				case MULTIPLICATOR_DAYS:
					FPuts(mulder,"INTERVALMULTIPLICATOR=DAYS\n");
					break;
			}

			DateToString(&buffer[100],&event->BeginDate,DATEFLAG_USETIME);
			sprintf(buffer,"BEGINTIME=%s\n",&buffer[100]);
			FPuts(mulder,buffer);

			DateToString(&buffer[100],&event->EndDate,DATEFLAG_USETIME);
			sprintf(buffer,"ENDTIME=%s\n",&buffer[100]);
			FPuts(mulder,buffer);

			sprintf(buffer,"DATETYPE=");
			switch(event->DateType)
			{
				case 0:
					strcat(buffer,"DATE");
					break;
				case 1:
					strcat(buffer,"SPACE");
					break;
				case 2:
					strcat(buffer,"MONTH");
					break;
				case 3:
					strcat(buffer,"DAILY");
					break;
			}
			strcat(buffer,"\n");
			FPuts(mulder,buffer);

			if (event->UseInterval)
				FPuts(mulder,"USEINTERVAL=TRUE\n");
			else
				FPuts(mulder,"USEINTERVAL=FALSE\n");

			if (event->Holiday==1)
				FPuts(mulder,"HOLIDAYS=ONLY\n");
			else
				if (event->Holiday==2)
					FPuts(mulder,"HOLIDAYS=DONOTHING\n");

				strcpy(buffer,"WEEKDAYS=");
			if (event->weekday[0])
				strcat(buffer,"MON ");
			if (event->weekday[1])
				strcat(buffer,"THU ");
			if (event->weekday[2])
				strcat(buffer,"WEN ");
			if (event->weekday[3])
				strcat(buffer,"TUR ");
			if (event->weekday[4])
				strcat(buffer,"FRI ");
			if (event->weekday[5])
				strcat(buffer,"SAT ");
			if (event->weekday[6])
				strcat(buffer,"SUN ");
			if (strlen(buffer)>9)
			{
				strcat(buffer,"\n");
				FPuts(mulder,buffer);
			}

			if (event->UseWeekday)
				FPuts(mulder,"USEWEEKDAYS=TRUE\n");
			else
				FPuts(mulder,"USEWEEKDAYS=FALSE\n");


			strcpy(buffer,"MONTH=");
			if (event->month[0])
				strcat(buffer,"JAN ");
			if (event->month[1])
				strcat(buffer,"FEB ");
			if (event->month[2])
				strcat(buffer,"MRZ ");
			if (event->month[3])
				strcat(buffer,"APR ");
			if (event->month[4])
				strcat(buffer,"MAY ");
			if (event->month[5])
				strcat(buffer,"JUN ");
			if (event->month[6])
				strcat(buffer,"JUL ");
			if (event->month[7])
				strcat(buffer,"AUG ");
			if (event->month[8])
				strcat(buffer,"SEP ");
			if (event->month[9])
				strcat(buffer,"OCT ");
			if (event->month[10])
				strcat(buffer,"NOV ");
			if (event->month[11])
				strcat(buffer,"DEC ");
			if (strlen(buffer)>6)
			{
				strcat(buffer,"\n");
				FPuts(mulder,buffer);
			}

			strcpy(buffer,"EVENTTYPE=");
			if (event->EventType==0)
				strcat(buffer,"NOVIA");
			if (event->EventType==1)
				strcat(buffer,"DOS");
			if (event->EventType==2)
				strcat(buffer,"AREXX");
			strcat(buffer,"\n");

			FPuts(mulder,buffer);

			FPuts(mulder,"ENDEVENT\n\n");

			event=event->ln_Succ;
		}
		Close(mulder);
	}
	else
	printf("can't open %s\n",buffer);
}


void LoadMailServers()
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	char *buffer;
	BPTR mulder;

	NewList((List *)&mainportconfig->POP3ServerList);
	NewList((List *)&mainportconfig->LocalMailServerList);

	if ((buffer=AllocVec(1024,MEMF_ANY|MEMF_CLEAR)))
	{
		sprintf(buffer,"%ssys.uucp_localmail_servers",mainportconfig->sysdirs.sysdata);
		if ((mulder = Open(buffer,MODE_OLDFILE)))
		{
			struct INet_LocalMailServer *server=NULL;
			struct INet_MailUser *user=NULL;
			char quit=FALSE;
			ULONG strlength;
			char start=FALSE;
			ULONG	linecounter=1;

			while (!quit && FGets(mulder,buffer,1023)) // Nicht 1024, wegen eines Bugs in der dos.library V37
			{
				strlength=strlen(buffer);
				if (searchstr(buffer,"STARTSERVER"))
				{
					if (!start)
					{
						start=TRUE;
						if ((server=AllocVec(sizeof(INet_LocalMailServer),MEMF_ANY|MEMF_CLEAR)))
						{
							NewList((List *)&server->Users);
						}
						
						else
						{
							quit=TRUE;
						}
					}
					else
					{
						printf("localmainserverlist error in line %d, STARTSERVER ALREADY.\n",linecounter);
					}
				}
				if (start)
				{
					if (searchstr(buffer,"NAME"))
					{
						if (searchstr(buffer,"UserName"))
						{
							if (searchstr(buffer,"Flag"))
							{
							}
							else
							{
								if (user)
								{
									mystrcpy(user->Name,buffer,40)
								}
								else
								{
									printf("localmainserverlist error in line %d, Name STARTUSER.\n",linecounter);
								}
							}
						}
						else
						{
							if (searchstr(buffer,"INVALIDFILENAME"))
							{
								mystrcpy(server->InvalidFilename,buffer,120)
							}
							else
							{
								mystrcpy(server->Name,buffer,40);
							}
						}
					}
					if (searchstr(buffer,"DOMAIN"))
					{
						mystrcpy(server->Domain,buffer,40)
					}
					if (searchstr(buffer,"SMTPHOST"))
					{
						char *pos=&buffer[8];
						char newpos=0;
						ULONG ulong;
						while (*pos && *pos!=' ' && *pos!='=' && *pos!=':')
							pos++;
						pos++;
						while (*pos && *pos!=':' && newpos<40)
						{
							server->SMTPHost[newpos]=*pos;
							newpos++;
							pos++;
						}
						if (*pos==':')
						{
							StrToLong(pos+1,(LONG *)&ulong);
							server->SMTPPort=(UWORD)ulong;
						}
					}
					if (searchstr(buffer,"POP3HOST"))
					{
						char *pos=&buffer[8];
						char newpos=0;
						ULONG ulong;
						while (*pos && *pos!=' ' && *pos!='=' && *pos!=':')
							pos++;
						pos++;
						while (*pos && *pos!=':' && newpos<40)
						{
							server->POP3Host[newpos]=*pos;
							newpos++;
							pos++;
						}
						if (*pos==':')
						{
							StrToLong(pos+1,(LONG *)&ulong);
							server->POP3Port=(UWORD)ulong;
						}
					}
					if (searchstr(buffer,"FLAGS"))
					{
						if (searchstr(buffer,"USERFLAGS"))
						{
							if (user)
							{
								if (searchstr(buffer,"UseBBSUserName"))
									user->Flag_UseBBSUserName=TRUE;
								if (searchstr(buffer,"UseBBSPassword"))
									user->Flag_UseBBSPassword=TRUE;
							}
						}
						else
						{
							if (searchstr(buffer,"Allow8Bit"))
							{
								server->Flag_Allow8Bit=TRUE;
							}
							if (searchstr(buffer,"UseAPOP"))
							{
								server->Flag_UseAPOP=TRUE;
							}
							if (searchstr(buffer,"ExternalServer"))
							{
								server->Flag_ExternalServer=TRUE;
							}
						}
					}
					if (searchstr(buffer,"INVALIDMAILSUBJECT"))
					{
						mystrcpy(server->InvalidMailSubject,buffer,80)
					}
					if (searchstr(buffer,"INVALIDADDRESS"))
					{
						mystrcpy(server->InvalidAddress,buffer,80)
					}
					if (searchstr(buffer,"INBOUND"))
					{
						mystrcpy(server->InboundPath,buffer,80)
					}
					if (searchstr(buffer,"OUTBOUND"))
					{
						mystrcpy(server->OutboundPath,buffer,70)
					}
					if (searchstr(buffer,"STARTUSER"))
					{
						if (user)
						{
							printf("localmainserverlist error in line %d, STARTUSER.\n",linecounter);							
						}
						else
						{
							if ((user=AllocVec(sizeof(INet_MailUser),MEMF_ANY|MEMF_CLEAR)))
							{
							}
							else
								quit=TRUE;
						}
					}
					if (searchstr(buffer,"IPNumber"))
					{
						if (user)
						{
							char *pos=&buffer[8];
							while ((*pos<'0' | *pos>'9') && *pos)
								pos++;
							StrToLong(pos+1,(LONG *)&user->IPNumber);
						}
						else
						{
							printf("localmainserverlist error in line %d, IPNumber STARTUSER.\n",linecounter);
						}
					}
					if (searchstr(buffer,"EXTERNALUSER"))
					{
						if (user)
							user->UserType=1;
					}
					if (searchstr(buffer,"POPID"))
					{
						if (user)
						{
							mystrcpy(user->POPID,buffer,40)
						}
						else
						{
							printf("localmainserverlist error in line %d, POPID STARTUSER.\n",linecounter);
						}
					}
					if (searchstr(buffer,"POPPASSWORD"))
					{
						if (user)
						{
							mystrcpy(user->POPPassword,buffer,40)
						}
						else
						{
							printf("localmainserverlist error in line %d, POPPASSWORD STARTUSER.\n",linecounter);
						}
					}
					if (searchstr(buffer,"HOLDMAILS"))
					{
						if (user)
						{
							ULONG ulong;
							char *pos=&buffer[9];
							while ((*pos<'0' | *pos>'9') && *pos)
								pos++;
							StrToLong(pos,(LONG *)&ulong);
							user->HoldMails=(UWORD)ulong;
						}
						else
						{
							printf("localmainserverlist error in line %d, HOLDMAILS STARTUSER.\n",linecounter);
						}
					}
					if (searchstr(buffer,"MaxEMails"))
					{
						if (user)
						{
							ULONG ulong;
							char *pos=&buffer[9];
							while ((*pos<'0' | *pos>'9') && *pos)
								pos++;
							StrToLong(pos,(LONG *)&ulong);
							user->MaxEMails=(UWORD)ulong;
						}
						else
						{
							printf("localmainserverlist error in line %d, MaxEMails STARTUSER.\n",linecounter);
						}
					}
					if (searchstr(buffer,"MailBoxSpace"))
					{
						if (user)
						{
							char *pos=&buffer[9];
							while ((*pos<'0' | *pos>'9') && *pos)
								pos++;
							StrToLong(pos,(LONG *)&user->Space);
						}
						else
						{
							printf("localmainserverlist error in line %d, MailBoxSpace STARTUSER.\n",linecounter);
						}
					}
				}
				if (searchstr(buffer,"END"))
				{
					if (start)
					{
						if (searchstr(buffer,"ENDUSER"))
						{
							if (user)
							{
								AddTail((List *)&server->Users,(Node *)user);
								user=NULL;
							}
							else
							{
								printf("localmainserverlist error in line %d, END STARTUSER.\n",linecounter);
							}
						}
						else
						{					
							server->Status=TRUE;
							AddTail((List *)&mainportconfig->LocalMailServerList,(Node *)server);
							start=FALSE;
							if (user)
							{
								AddTail((List *)&server->Users,(Node *)user);
								user=NULL;
							}
						}
					}
					else
					{
						printf("localmailserverlist error in line %d, END.\n",linecounter);
					}
				}
				linecounter++;
			}
			Close(mulder);
		}
		else
		{
			printf("can't load localmailserverlist. unable to open %s.\n",buffer);
		}

		sprintf(buffer,"%ssys.uucp_pop3_servers",mainportconfig->sysdirs.sysdata);
		if ((mulder = Open(buffer, MODE_OLDFILE)))
		{
			struct INet_POP3Server *server=NULL;
			char quit=FALSE;
			ULONG strlength;
			char start = FALSE;
			ULONG	linecounter=1;

			while (!quit && FGets(mulder,buffer,1023)) // Nicht 1024, wegen eines Bugs in der dos.library V37
			{
				strlength = strlen(buffer);
				if (!start)
				{
					if (searchstr(buffer, "STARTSERVER"))
					{
						if (!start)
						{
							if ((server = AllocVec(sizeof(struct INet_POP3Server), MEMF_ANY | MEMF_CLEAR)))
							{
								start = TRUE;
							}
							else
							{
								quit=TRUE;
							}
						}
						else
						{
							printf("pop3mainserverlist error in line %d, STARTSERVER ALREADY.\n",linecounter);
						}
					}
				}
				if (start)
				{
					if (searchstr(buffer,"NAME"))
					{
						if (searchstr(buffer,"INVALIDFILENAME"))
						{
							mystrcpy(server->InvalidFilename,buffer,120)
						}
						else
						{
							mystrcpy(server->Name,buffer,40);
						}
					}
					if (searchstr(buffer,"USER"))
					{
						mystrcpy(server->Username,buffer,80);						
					}
					if (searchstr(buffer,"PASSWORD"))
					{
						mystrcpy(server->Password,buffer,80);						
					}
					if (searchstr(buffer,"DOMAIN"))
					{
						mystrcpy(server->Domain,buffer,120)
					}
					if (searchstr(buffer,"SMTPHOST"))
					{
						mystrcpy(server->SMTPHost,buffer,80)
					}
					if (searchstr(buffer,"POP3HOST"))
					{
						mystrcpy(server->POP3Host,buffer,80)
					}
					if (searchstr(buffer,"FLAGS"))
					{
						{
							if (searchstr(buffer,"8Bit"))
							{
								server->Flag_8Bit=TRUE;
							}
							if (searchstr(buffer,"UseAPOP"))
							{
								server->Flag_UseAPOP=TRUE;
							}
							if (searchstr(buffer,"ExternalServer"))
							{
								server->Flag_ExternalServer=TRUE;
							}
							if (searchstr(buffer,"DeleteMails"))
							{
								server->Flag_DeleteMails=TRUE;
							}
						}
					}
					if (searchstr(buffer,"INVALIDMAILSUBJECT"))
					{
						mystrcpy(server->InvalidMailSubject,buffer,80)
					}
					if (searchstr(buffer,"INVALIDADDRESS"))
					{
						mystrcpy(server->InvalidAddress,buffer,80)
					}
					if (searchstr(buffer,"INBOUND"))
					{
						mystrcpy(server->InboundPath,buffer,80)
					}
					if (searchstr(buffer,"OUTBOUND"))
					{
						mystrcpy(server->OutboundPath,buffer,80)
					}
					if (searchstr(buffer,"END"))
					{
						server->Status = TRUE;
						AddTail((List *)&mainportconfig->POP3ServerList,(Node *)server);
						start = FALSE;
					}
				}
				ClearMemQuick(buffer,100);
				linecounter++;
			}
			Close(mulder);
		}
		else
		{
			printf("can't load pop3_mailserverlist. unable to open %s.\n",buffer);
		}





		FreeVec(buffer);
	}
	else
	{
		printf("can't open ENVARC:novia_mainportconfig.\n");
	}



}

void SaveMailServers()
{
	char buffer[200];
	BPTR mulder;
	INet_LocalMailServer *server=NULL;
	INet_POP3Server *popserver=NULL;

	sprintf(buffer,"%ssys.uucp_localmail_servers",mainportconfig->sysdirs.sysdata);
	if ((mulder=Open(buffer,MODE_NEWFILE)))
	{
		struct INet_MailUser *user;
		server=(INet_LocalMailServer *)mainportconfig->LocalMailServerList.mlh_Head;
		while (server->ln_Succ)
		{
			sprintf(buffer,"STARTSERVER (%d)\n",server->IDNumber);
			FPuts(mulder,buffer);

			sprintf(buffer,"	NAME=%s\n",server->Name);
			FPuts(mulder,buffer);

			sprintf(buffer,"	STATUS=%d\n",server->Status);
			FPuts(mulder,buffer);

			sprintf(buffer,"	DOMAIN=%s\n",server->Domain);
			FPuts(mulder,buffer);

			sprintf(buffer,"	SMTPHOST=%s:%d\n",server->SMTPHost,server->SMTPPort);
			FPuts(mulder,buffer);

			sprintf(buffer,"	POP3HOST=%s:%d\n",server->POP3Host,server->POP3Port);
			FPuts(mulder,buffer);

			FPuts(mulder,"	FLAGS=");
			if (server->Flag_Allow8Bit)
				FPuts(mulder,"ALLOW8BIT ");

			if (server->Flag_UseAPOP)
				FPuts(mulder,"USEAPOP ");

			if (server->Flag_ExternalServer)
				FPuts(mulder,"EXTERNALSERVER");

			FPuts(mulder,"\n");

			sprintf(buffer,"	INVALIDMAILSUBJECT=%s\n",server->InvalidMailSubject);
			FPuts(mulder,buffer);

			sprintf(buffer,"	INVALIDADDRESS=%s\n",server->InvalidAddress);
			FPuts(mulder,buffer);

			sprintf(buffer,"	INVALIDFILENAME=%s\n",server->InvalidFilename);
			FPuts(mulder,buffer);

			sprintf(buffer,"	INBOUND=%s\n",server->InboundPath);
			FPuts(mulder,buffer);

			sprintf(buffer,"	Outbound=%s\n",server->OutboundPath);
			FPuts(mulder,buffer);

			user=(INet_MailUser *)server->Users.mlh_Head;
			while (user->ln_Succ)
			{
				sprintf(buffer,"\n	STARTUSER (%d)\n",user->IDNumber);
				FPuts(mulder,buffer);

				sprintf(buffer,"		IPNumber=%d\n",user->IPNumber);
				FPuts(mulder,buffer);

				if (user->UserType==0)
					FPuts(mulder,"		UserType=LOCALUSER\n");
				else
					FPuts(mulder,"		UserType=EXTERNALUSER\n");

				sprintf(buffer,"		UserName=%s\n",user->Name);
				FPuts(mulder,buffer);

				sprintf(buffer,"		Status=%d\n",user->Status);
				FPuts(mulder,buffer);

				sprintf(buffer,"		POPID=%s\n",user->POPID);
				FPuts(mulder,buffer);

				sprintf(buffer,"		POPPASSWORD=%s\n",user->POPPassword);
				FPuts(mulder,buffer);

				sprintf(buffer,"		HoldMails=%i\n",user->HoldMails);
				FPuts(mulder,buffer);

				sprintf(buffer,"		MaxEMails=%i\n",user->MaxEMails);
				FPuts(mulder,buffer);

				sprintf(buffer,"		MailBoxSpace=%i\n",user->Space);
				FPuts(mulder,buffer);
				
				if (user->Flag_UseBBSUserName | user->Flag_UseBBSPassword)
				{
					strcpy(buffer,"		USERFLAGS=");
					if (user->Flag_UseBBSUserName)
						strcat(buffer,"UseBBSUserName ");
					if (user->Flag_UseBBSPassword)
						strcat(buffer,"UseBBSPassword");
					strcat(buffer,"\n");
					FPuts(mulder,buffer);
				}


				FPuts(mulder,"	ENDUSER\n");

				user=user->ln_Succ;
			}	

			FPuts(mulder,"END\n\n");
			server=server->ln_Succ;
		}
		Close(mulder);
	}

	sprintf(buffer,"%ssys.uucp_pop3_servers",mainportconfig->sysdirs.sysdata);
	if ((mulder=Open(buffer,MODE_NEWFILE)))
	{
		popserver=(INet_POP3Server *)mainportconfig->POP3ServerList.mlh_Head;
		while (popserver->ln_Succ)
		{
			sprintf(buffer,"STARTSERVER (%d)\n",popserver->IDNumber);
			FPuts(mulder,buffer);

			sprintf(buffer,"NAME=%s\n",popserver->Name);
			FPuts(mulder,buffer);

			sprintf(buffer,"STATUS=%d\n",popserver->Status);
			FPuts(mulder,buffer);

			sprintf(buffer,"DOMAIN=%s\n",popserver->Domain);
			FPuts(mulder,buffer);

			sprintf(buffer,"USER=%s\n",popserver->Username);
			FPuts(mulder,buffer);

			sprintf(buffer,"PASSWORD=%s\n",popserver->Password);
			FPuts(mulder,buffer);

			sprintf(buffer,"SMTPHOST=%s\n",popserver->SMTPHost);
			FPuts(mulder,buffer);

			sprintf(buffer,"POP3HOST=%s\n",popserver->POP3Host);
			FPuts(mulder,buffer);

			FPuts(mulder,"FLAGS=");
			if (popserver->Flag_8Bit)
				FPuts(mulder,"8BIT ");

			if (popserver->Flag_UseAPOP)
				FPuts(mulder,"USEAPOP ");

			if (popserver->Flag_ExternalServer)
				FPuts(mulder,"EXTERNALSERVER ");

			if (popserver->Flag_DeleteMails)
				FPuts(mulder,"DELETEMAILS ");

			FPuts(mulder,"\n");

			sprintf(buffer,"INVALIDMAILSUBJECT=%s\n",popserver->InvalidMailSubject);
			FPuts(mulder,buffer);

			sprintf(buffer,"INVALIDADDRESS=%s\n",popserver->InvalidAddress);
			FPuts(mulder,buffer);

			sprintf(buffer,"INVALIDFILENAME=%s\n",popserver->InvalidFilename);
			FPuts(mulder,buffer);

			sprintf(buffer,"INBOUND=%s\n",popserver->InboundPath);
			FPuts(mulder,buffer);

			sprintf(buffer,"OUTBOUND=%s\n",popserver->OutboundPath);
			FPuts(mulder,buffer);

			FPuts(mulder,"END\n\n");
			popserver=popserver->ln_Succ;
		}
		Close(mulder);
	}
}


void mystrcpy(char *dest,char *src,ULONG length)
{
	ULONG counter=0;
	while (*src && (*src==' ' | *src==9 | isalnum(*src) ))
		src++;

	while (*src && (*src==' ' | *src=='=' | *src==':' | *src==9 ))
		src++;
	while (*src && *src!='\n' && *src!='\r' && counter<length)
	{
		*dest++=*src++;
		counter++;
	}
	if (counter>=length)
		*(dest-1)=0;
	else
		*dest=0;

}


void StartReminder()
{
	char error=FALSE;
	struct PortData *mport=(PortData *)MainTask->tc_UserData;
	char buffer[200];
	struct TagItem ProcTagList[] =
	{
		{	NP_Entry,		(ULONG)reminder_main},
		{	NP_Name,			(ULONG)"novia_reminder"},
		{	NP_StackSize,	20000},
		{	TAG_DONE,		TAG_DONE},
	};
	CreateNewProc(ProcTagList);
}

void reminder_main()
{
	char quit=FALSE;
	ULONG sigs=0;
	ULONG sb_reminder_eventport=NULL;
	ULONG	sb_timerport=NULL;
	char buffer[200];
	Event *event;
	char *reminder_eventportname="novia_reminder_control_port";
	struct MsgPort *timerport=CreateMsgPort();
	struct timerequest *timereq=NULL;

	reminder_eventport=CreateMsgPort();
	if (reminder_eventport && timerport)
	{
		sb_reminder_eventport=1<<reminder_eventport->mp_SigBit;
		sb_timerport=1<<timerport->mp_SigBit;
		reminder_eventport->mp_Node.ln_Name=reminder_eventportname;
		AddPort(reminder_eventport);

		if ((timereq=CreateIORequest(timerport,sizeof(EventRequest))))
		{
			if (OpenDevice("timer.device",UNIT_VBLANK,(IORequest *)timereq,0))
			{
				printf("can't open timer.device.\nUnable to start Reminder.\n");
				quit=TRUE;
			}
			else
			{
				TimerBase = (struct Library *)timereq->tr_node.io_Device;
			}
		}
		else
			quit=TRUE;
	}
	else
		quit=TRUE;

//	GetTime(struct Date *date);

	if (!quit)
	{
		timereq->tr_time.tv_secs=5;
		timereq->tr_time.tv_micro=0;
		timereq->tr_node.io_Command=TR_ADDREQUEST;
		SendIO((IORequest *)timereq);

		GetTime(&actualdate);

		event=(Event *)mainportconfig->EventList.mlh_Head;
		while (event->ln_Succ)
		{
			if ((event->eventreq=CreateIORequest(timerport,sizeof(EventRequest))))
			{
				ULONG datecode = 0;
				event->eventreq->tr_node.io_Device  = timereq->tr_node.io_Device;
				event->eventreq->tr_node.io_Unit    = timereq->tr_node.io_Unit;
				event->eventreq->tr_node.io_Command = TR_ADDREQUEST;
				event->eventreq->tr_time.tv_micro   = 0;
				event->init=TRUE;

				datecode = CalcEvent(event);
				if (datecode)
				{
					if (event->NextEvent.datecode)
						CopyMem(&event->NextEvent,&event->LastEvent,sizeof(Date));
					event->NextEvent.datecode=datecode;
					Datecode2Date(&event->NextEvent);

					if (datecode > actualdate.datecode)
					{
						event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
						SendIO((IORequest *)event->eventreq);
						event->Running=TRUE;
					}
					else
					{
						CopyMem(&actualdate,&event->NextEvent,sizeof(Date));
						datecode = CalcEvent(event);
						if (datecode <= actualdate.datecode)
						{
							event->Running=FALSE;
						}
						else
						{
//							CopyMem(&actualdate,&event->LastEvent,sizeof(Date));

							event->NextEvent.datecode = datecode;
							Datecode2Date(&event->NextEvent);

							ExecuteEvent(event);

							event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
							SendIO((IORequest *)event->eventreq);
							event->Running=TRUE;
						}
					}
				}
				else
				{
				}
			}
			event=event->ln_Succ;
		}
		SendEventMsg(main_eventport,reminder_eventport, 0, GUIMSG_REDRAW_REMINDERLIST);
	}
	while (!quit)
	{
		sigs=Wait(sb_reminder_eventport|sb_timerport);
		if ((sb_reminder_eventport & sigs))
		{
			struct EventMessage *msg;
			while ((msg=(EventMessage *)GetMsg(reminder_eventport)))
			{
				switch (msg->msg_type)
				{
					case EVENTMSGTYPE_QUIT:
						quit=TRUE;
						break;
					case GUIMSG_REDRAW_REMINDERLIST:
						break;
				}
				if (msg->msg.mn_ReplyPort!=reminder_eventport)
					ReplyMsg((Message *)msg);
				else
				{
					FreeVec((Message *)msg);
				}
			}
		}
		if ((sb_timerport & sigs))
		{
			struct EventRequest *eventreq;
			while ((eventreq = (EventRequest *)GetMsg(timerport)))
			{
				if (eventreq == (EventRequest *)timereq)
				{
					if (CheckIO((IORequest *)timereq))
					{
						WaitIO((IORequest *)timereq);
						timereq->tr_time.tv_secs=5;
						SendIO((IORequest *)timereq);
						GetTime(&actualdate);
						event=(Event *)mainportconfig->EventList.mlh_Head;
						while (event->ln_Succ)
						{
							if (event->init==FALSE)
							{
								if ((event->eventreq=CreateIORequest(timerport,sizeof(EventRequest))))
								{
									ULONG datecode = 0;
									event->eventreq->tr_node.io_Device  = timereq->tr_node.io_Device;
									event->eventreq->tr_node.io_Unit    = timereq->tr_node.io_Unit;
									event->eventreq->tr_node.io_Command = TR_ADDREQUEST;
									event->eventreq->tr_time.tv_micro   = 0;
									event->init=TRUE;
								}
							}
							if (event->Running==FALSE && event->init==TRUE)
							{
								ULONG datecode;
								datecode = CalcEvent(event);
								if (datecode)
								{
									if (event->NextEvent.datecode)
										CopyMem(&event->NextEvent,&event->LastEvent,sizeof(Date));
									event->NextEvent.datecode=datecode;
									Datecode2Date(&event->NextEvent);

									if (datecode > actualdate.datecode)
									{
										event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
										SendIO((IORequest *)event->eventreq);
										event->Running=TRUE;
									}
									else
									{
										CopyMem(&actualdate,&event->NextEvent,sizeof(Date));
										datecode = CalcEvent(event);
										if (datecode <= actualdate.datecode)
											event->Running=FALSE;
										else
										{
											CopyMem(&actualdate,&event->LastEvent,sizeof(Date));

											event->NextEvent.datecode = datecode;
											Datecode2Date(&event->NextEvent);

											ExecuteEvent(event);

											event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
											SendIO((IORequest *)event->eventreq);
											event->Running=TRUE;
										}
									}
								}
							}
							event=event->ln_Succ;
						}
					}
				}
				else
				{
/*
					if ((event=eventreq->event))
					{
						AbortIO((IORequest *)eventreq);
						if (CheckIO((IORequest *)eventreq))
						{
							GetTime(&actualdate);
							CalcEvent(event);
							if (event->NextEvent.datecode > actualdate.datecode)
							{
								event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
								SendIO((IORequest *)event->eventreq);
							}
							SendEventMsg(main_eventport,reminder_eventport, 0, GUIMSG_REDRAW_REMINDERLIST);
							ExecuteEvent(event);
						}
						else
							printf("%s not on\n",event->Name);
					}
*/


					event=(Event *)mainportconfig->EventList.mlh_Head;
					while (event->ln_Succ)
					{
						if (event->eventreq && eventreq == event->eventreq)
						{
							if (CheckIO((IORequest *)event->eventreq))
							{
								ULONG datecode;

								GetTime(&actualdate);
								datecode = CalcEvent(event);
								CopyMem(&event->NextEvent,&event->LastEvent,sizeof(Date));
								
								if (datecode)
								{
									if (datecode > actualdate.datecode)
									{
										event->NextEvent.datecode = datecode;
										Datecode2Date(&event->NextEvent);

										event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
										SendIO((IORequest *)event->eventreq);
										event->Running=TRUE;
									}
									else
									{
										CopyMem(&actualdate,&event->NextEvent,sizeof(Date));
										datecode = CalcEvent(event);
										if (datecode <= actualdate.datecode)
										{
											event->Running=FALSE;
										}
										else
										{
											event->NextEvent.datecode = datecode;
											Datecode2Date(&event->NextEvent);
											event->eventreq->tr_time.tv_secs = event->NextEvent.datecode - actualdate.datecode;
											SendIO((IORequest *)event->eventreq);
											event->Running=TRUE;
										}
									}
								}

								ExecuteEvent(event);
								SendEventMsg(main_eventport,reminder_eventport, 0, GUIMSG_REDRAW_REMINDERLIST);
							}
						}
						event=event->ln_Succ;
					}

				}
			}
		}
	}
	AbortIO((IORequest *)timereq);
	WaitIO((IORequest *)timereq);
	event=(Event *)mainportconfig->EventList.mlh_Head;
	while (event->ln_Succ)
	{
		if (event->init)
		{
			event->init=FALSE;
			event->Running=FALSE;
			if (event->eventreq)
			{
				AbortIO((IORequest *)event->eventreq);
				WaitIO((IORequest *)event->eventreq);
			}
			if (event->eventreq)
				DeleteIORequest((IORequest *)event->eventreq);
		}
		event=event->ln_Succ;
	}

	if (reminder_eventport)
	{
		if (FindPort(reminder_eventportname))
			RemPort(reminder_eventport);
		DeleteMsgPort(reminder_eventport);
		reminder_eventport=NULL;
	}
	if (timereq)
	{
		CloseDevice((IORequest *)timereq);
		DeleteIORequest((IORequest *)timereq);
	}
	if (timerport)
		DeleteMsgPort(timerport);
}

ULONG CalcEvent(Event *event)
{
	ULONG result = 0;
	struct Date *nextdate=&event->NextEvent;
	struct Date *lastdate=&event->LastEvent;
	struct Date *edate=&event->BeginDate;

	switch (event->DateType)
	{
		case DATETYPE_DATE:

			if (event->UseInterval==FALSE | (!nextdate->datecode))
			{
				result=event->BeginDate.datecode;
			}
			else
			{
				ULONG nexttime = nextdate->hours*3600 + nextdate->mins*60 + nextdate->secs + event->IntervalTime * event->Multiplicator;
				ULONG	endtime = event->EndDate.hours*3600 + event->EndDate.mins*60 + event->EndDate.secs;
				if (nexttime < endtime)
					result = nextdate->datecode + event->IntervalTime * event->Multiplicator;

			}
			break;

		case DATETYPE_SPACE:
		
			if (event->UseInterval==FALSE | (!nextdate->datecode))
			{
				UBYTE counter=0;
				if (event->UseWeekday)
				{
					UBYTE weekday;
					if (!nextdate->datecode)
						weekday=event->BeginDate.weekday;
					else
						weekday=nextdate->weekday;
					if (weekday<6)
						weekday++;
					else
						weekday=0;					
					while (counter<7 && !event->weekday[weekday])
					{
						counter++;
						if (weekday<6)
							weekday++;
						else
							weekday=0;
					}
					if (counter==7)counter=0;
				}
				if (nextdate->datecode==0)
					result=event->BeginDate.datecode + (counter * 86400);
				else
				{
					if ((nextdate->datecode + 86400 + (counter * 86400))<= event->EndDate.datecode)
						result = nextdate->datecode + ((counter + 1) * 86400);
				}
			}
			else
			{
				UBYTE counter=0;
				ULONG nexttime=nextdate->hours*3600 + nextdate->mins*60 + nextdate->secs + event->IntervalTime * event->Multiplicator;
				ULONG endtime=event->EndDate.hours*3600 + event->EndDate.mins*60 + event->EndDate.secs;
				if (event->UseWeekday)
				{
					UBYTE weekday=nextdate->weekday;
					if (weekday<6)
						weekday++;
					else
						weekday=0;					
					while (counter<7 && !event->weekday[weekday])
					{
						counter++;
						if (weekday<6)
							weekday++;
						else
							weekday=0;
					}
					if (counter==7)counter=0;
				}
				if (nexttime <= endtime)
				{
					result = nextdate->datecode+event->IntervalTime * event->Multiplicator;
				}
				else
				{
					if (event->IntervalTime * event->Multiplicator <= 86400)
					{
						if ((nextdate->datecode + event->IntervalTime * event->Multiplicator + counter * 86400) < event->EndDate.datecode)
						{
							nexttime=86400-(nextdate->hours*3600 + nextdate->mins*60 + nextdate->secs);
							result = nextdate->datecode + nexttime + (event->BeginDate.hours*3600 +event->BeginDate.mins*60+event->BeginDate.secs) + (counter * 86400);
						}
					}
					else
					{
						if ((nextdate->datecode + event->IntervalTime * event->Multiplicator) < event->EndDate.datecode)
						{
							UBYTE counter=0;
							if (event->UseWeekday)
							{
								ULONG days = ((event->IntervalTime * event->Multiplicator)/86400) * 86400;
								ULONG restdays = days-(days/7)*7;
								ULONG weekday = nextdate->weekday;

								if (weekday+restdays<7)
									weekday=weekday+restdays;
								else
									weekday=weekday+restdays-7;

								weekday=nextdate->weekday;
								if (weekday<6)
									weekday++;
								else
									weekday=0;					

								while (counter<7 && !event->weekday[weekday])
								{
									counter++;
									if (weekday<6)
										weekday++;
									else
										weekday=0;
								}
								if (counter==7)counter=0;
							}
							if ((nextdate->datecode + event->IntervalTime * event->Multiplicator + counter * 86400) <= event->EndDate.datecode)
							{
								struct Date mydate;
								mydate.datecode=nextdate->datecode + event->IntervalTime * event->Multiplicator;
								Datecode2Date(&mydate);
								mydate.hours = event->BeginDate.hours;
								mydate.mins = event->BeginDate.mins;
								mydate.secs = event->BeginDate.secs;
								CalcDatecode(&mydate);
								result = mydate.datecode;
							}
						}
					}
				}
			}
			break;
		case DATETYPE_MONTH:



			break;
		case DATETYPE_DAILY:
			if (event->UseInterval==FALSE | (!nextdate->datecode))
			{
				UBYTE counter=0;
				if (event->UseWeekday)
				{
					UBYTE weekday=nextdate->weekday;
					if (weekday<6)
						weekday++;
					else
						weekday=0;
							
					while (counter<7 && !event->weekday[weekday])
					{
						counter++;
						if (weekday<6)
							weekday++;
						else
							weekday=0;
					}
					if (counter==7)counter=0;
				}
				if (nextdate->datecode==0)
				{
					struct Date mydate;
					CopyMem(&actualdate,&mydate,sizeof(Date));
					if (event->UseInterval==FALSE)
					{
						ULONG actualtime = actualdate.hours*3600 + actualdate.mins*60 + actualdate.secs;
						ULONG begintime  = event->BeginDate.hours*3600 + event->BeginDate.mins*60 + event->BeginDate.secs;
						if (actualtime>begintime | counter)
							result = mydate.datecode + (86400-actualtime) + begintime + (counter*86400);
					}
					else
					{
						if (event->IntervalTime * event->Multiplicator <= 86400)
						{
							ULONG actualtime = actualdate.hours*3600 + actualdate.mins*60 + actualdate.secs;
							ULONG endtime  = event->EndDate.hours*3600 + event->EndDate.mins*60 + event->EndDate.secs;
							if (actualtime>endtime | counter)
								result = mydate.datecode + (86400-actualtime) + (event->BeginDate.hours*3600 + event->BeginDate.mins*60 + event->BeginDate.secs)  + (counter*86400);
						}
						else
						{
							UBYTE counter = 0;
							if (event->UseWeekday)
							{
								ULONG days     = ((event->IntervalTime * event->Multiplicator)/86400) * 86400;
								ULONG restdays = days-(days/7)*7;
								ULONG weekday  = nextdate->weekday;;
			
								if (weekday+restdays<7)
									weekday=weekday+restdays;
								else
									weekday=weekday+restdays-7;
			
								if (weekday<6)
									weekday++;
								else
									weekday=0;
										
								while (counter<7 && !event->weekday[weekday])
								{
									counter++;
									if (weekday<6)
										weekday++;
									else
										weekday=0;
								}
								if (counter==7)counter=0;
							}
	
							if ((mydate.datecode + event->IntervalTime * event->Multiplicator + counter * 86400) < event->EndDate.datecode)
							{
								mydate.datecode = mydate.datecode + event->IntervalTime * event->Multiplicator + counter * 86400;
								Datecode2Date(&mydate);
								mydate.hours = event->BeginDate.hours;
								mydate.mins  = event->BeginDate.mins;
								mydate.secs  = event->BeginDate.secs;
								CalcDatecode(&mydate);
								result = mydate.datecode;
							}
						}
					}
				}
				else
					result = nextdate->datecode + 86400 + counter * 86400;
			}
			else
			{
				ULONG nexttime=nextdate->hours*3600 + nextdate->mins*60 + nextdate->secs + event->IntervalTime * event->Multiplicator;
				ULONG endtime=event->EndDate.hours*3600 + event->EndDate.mins*60 + event->EndDate.secs;

				if (nexttime <= endtime)
					result = nextdate->datecode+event->IntervalTime * event->Multiplicator;
				else
				{
					UBYTE counter=0;
					if (event->UseWeekday)
					{
						UBYTE weekday;
						if (!nextdate->datecode)
							weekday=event->BeginDate.weekday;
						else
							weekday=nextdate->weekday;
						if (weekday<6)
							weekday++;
						else
							weekday=0;					
						while (counter<7 && !event->weekday[weekday])
						{
							counter++;
							if (weekday<6)
								weekday++;
							else
								weekday=0;
						}
						if (counter==7)counter=0;
					}
					result = nextdate->datecode + (86400 - (nexttime - event->IntervalTime * event->Multiplicator)) + (event->BeginDate.hours*3600 + event->BeginDate.mins*60 + event->BeginDate.secs) + counter*86400;
				}
			}
			break;
	}
	return result;
}

void ExecuteEvent(Event *event)
{
//	printf("Execute event: %s next: %d last: %d actual: %d\n",event->Name,event->NextEvent.datecode,event->LastEvent.datecode,actualdate.datecode);
}

void SendOnlineMessage(PortData *cport)
{
	char buffer[200];
	ULONG		touser	= 0;
	PortData *toport	= NULL;
	BOOL		broadcast= FALSE;
	if (!strlen(cport->argstring[1]))
	{
		ioprintf("Port#, or UserHandle (* = BROADCAST MESSAGE): ");
		Getstring(buffer,0,50,0,0);
		if (strlen(buffer))
		{
			StrToLong(buffer,(LONG *)&touser);
			if (touser)
				toport = FindClient(touser);
			else
			{
				if ((touser = FindUser(buffer)))
					toport = FindClientByUser(touser);
				else
					if (*buffer = '*')
						broadcast = TRUE;
			}
		}
	}
	else
	{
		StrToLong(cport->argstring[1],(LONG *)&touser);
		if (touser)
			toport = FindClient(touser);
		else
		{
			if ((touser = FindUser(cport->argstring[1])))
				toport = FindClientByUser(touser);
			else
				if (*cport->argstring[1] = '*')
					broadcast = TRUE;
		}
	}
	if (toport)
	{
		ioprintf("enter your msg:");
		Getstring(buffer,0,50,0,0);
		if (strlen(buffer))
		{
			struct OnLineMessage *olm;
			if ((olm = AllocVec(sizeof(OnLineMessage)+strlen(buffer)+1,MEMF_ANY|MEMF_CLEAR)))
			{
				olm->data_len	= strlen(buffer);
				olm->OLM_type	= OLM_TYPE_NORMAL;
				olm->ToUser		= touser;
				olm->FromUser  = cport->LocalUser.IDNumber;
				strcpy(olm->data, buffer);
				SendMsg(toport->LocalPort,olm,sizeof(OnLineMessage)+strlen(buffer)+1,MSG_COMMAND,CMD_OLM);
				ioprintf("Message sended.\n");
			}
			else
				ioprintf("can't send message, out of memory.\n");
		}
	}
	else
	{
		if (broadcast)
		{
			ioprintf("enter your msg:");
			Getstring(buffer,0,50,0,0);
			if (strlen(buffer))
			{
				struct OnLineMessage *olm;
				struct PortData2 *clientport = (PortData2 *)client_list->lh_Head;
				while (clientport->port_node.ln_Succ)
				{
					if (!clientport->port_address->ProgramClose && clientport->port_address->network.online)
					{
						if ((olm = AllocVec(sizeof(OnLineMessage)+strlen(buffer)+1,MEMF_ANY|MEMF_CLEAR)))
						{
							olm->data_len	= strlen(buffer);
							olm->OLM_type	= OLM_TYPE_BROADCAST;
							olm->ToUser		= touser;
							strcpy(olm->data, buffer);
							SendMsg(clientport->port_address->LocalPort,olm,sizeof(OnLineMessage)+strlen(buffer)+1,MSG_COMMAND,CMD_OLM);
							ioprintf("Message sended.\n");
						}
						else
							ioprintf("can't send message, out of memory.\n");
					}
					clientport=(PortData2 *)clientport->port_node.ln_Succ;
				}
			}
		}
	}
}


PortData *FindClient( ULONG PortID)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct PortData2 *nc=(PortData2 *)client_list->lh_Head;
	if (PortID>=0)
	{
		while (nc->port_node.ln_Succ)
		{
			if (nc->port_address->clientconfig.IDNumber==PortID)
				return nc->port_address;
			nc=(PortData2 *)nc->port_node.ln_Succ;
		}
	}
	return 0;		
}

PortData *FindClientByUser( ULONG UserID)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct PortData2 *nc=(PortData2 *)client_list->lh_Head;
	ioprintf("findClientByUser: %d\n",UserID);
	if (UserID)
	{
		while (nc->port_node.ln_Succ)
		{
			if (nc->port_address->LocalUser.IDNumber == UserID)
				return nc->port_address;
			nc=(PortData2 *)nc->port_node.ln_Succ;
		}
	}
	return 0;		
}

void Writechar(char i)
{
	char *p;
	*p = i;
	Writeio(p, 1);
}

void CR()
{
	char *p="[E[A";
	Writeio(p,-1);
}

