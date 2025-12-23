#define NOVIA_GUI_VERSION "$VER: NOVIA GUI-MUI-Frontend V0.13  ["__DATE__" "__TIME__"]"
#define PROGTYPE_NOVIA_GUI
#include <libraries/mui.h>
#include <libraries/gadtools.h>
#include <clib/muimaster_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/asl_protos.h>
#include <clib/icon_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/utility_protos.h>
#include <pragma/muimaster_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/noviasys_lib.h>
#include <stdlib.h>
#include <stdio.h>
#include <novia/novia_userdata.h>
#include <novia/novia_userlist.h>
#include <novia/novia_gui.h>
#include <novia/novia_config.h>
#include <novia/novia_misc.h>
#include <novia/novia_global.h>
#include <novia/novia_funcs.h>
#include <novia/novia_userdata.h>
#include <exec/memory.h>
#include <libraries/iffparse.h>
#include <dos/stdio.h>

#include "novia_gui_data.h"
#include "novia_baseview_data.h"

#define TESTTEXT "Testtext for Mailtext CustomClass\n\n*Bold* /Italics/ _Underline_ #Coloured# _/*Combination*/_\n\n\nQuoted Text:\n\n1>2>3>4>5> *Bold* /Italics/ _Underline_ #Coloured#\n\nURLs: http://home.pages.de/~Mailtext/\n\nTo clear things up a bit: (let + be *, /, _ or #)\n\n   +Text+\n   ^    ^\n   |    |\n   |    +- the terminating char\n   |\n   +- the introducing char\n\n-- \nolf@gmx.de"

#define ID_ActionURL   1
#define ID_ActionEMail 2

//#include <mui/demo.h>

#include <mui/Mailtext_mcc.h>
#include <mui/NListview_mcc.h>

#include <dos/rdargs.h>

//#define DEBUG

struct MyNode {
	struct MyNode 	*ln_Succ;
	struct MyNode	*ln_Pred;
	UBYTE			ln_Type;
	BYTE			ln_Pri;
	BYTE 			*ln_Name;
	ULONG			IPNumber;
};

#include "mcpp:lib/mui.c"
#define REG(x) register __ ## x

//#define ASM
//#define SAVEDS

char VER[]="$VER: NOVIA GUI-MUI-Frontend V0.14  ("__DATE__" "__TIME__")";

extern void GetX(Object *object,APTR pointer,ULONG ulong);
extern BOOL MainPortConfigToGUI(MainPortConfig *config);
extern BOOL GUIToMainPortConfig(MainPortConfig *config);
extern BOOL ClientConfigToGUI(ClientConfig *config);
extern void GUIToClientConfig(ClientConfig *config);
extern void INet_POP3ServerToGUI(INet_POP3Server *server);
extern void INet_LocalMailServerToGUI(INet_LocalMailServer *server);
extern void GUIToINet_POP3Server(INet_POP3Server *server);
extern void GUIToINet_LocalMailServer(INet_LocalMailServer *user);
extern void INet_LocalUserToGUI(INet_MailUser *user);
extern void GUIToINet_LocalUser(INet_MailUser *user);
extern void EventToGUI(struct Event *event);
extern void GUIToEvent(struct Event *event);
extern void ExecuteEvent(Event *event);
void UpdateBaselist(char *filename);
void ParrentBaselist();


ULONG ScreenmodeToStr(REG(a2) Object *object,REG(a1) ScreenModeRequester *asl);
LONG DisplayListcport(REG(a2) char **array,REG(a1) struct PortData *cport2); 
LONG DisplayListcportmin(REG(a2) char **array,REG(a1) struct PortData *cport2);
LONG DisplayListudata(REG(a2) char **array,REG(a1) struct UserNode *udata);
LONG DisplayListPOP3Server(REG(a2) char **array,REG(a1) INet_POP3Server *udata);
LONG DisplayListLocalMailServer(REG(a2) char **array,REG(a1) INet_LocalMailServer *udata);
LONG DisplayListMailserverUpdate(REG(a2) char **array,REG(a1) INet_LocalMailServer *udata);
LONG DisplayListMailserverUser(REG(a2) char **array,REG(a1) struct INet_MailUser *newuser);
LONG DisplayListPOP3Update(REG(a2) char **array,REG(a1) struct INet_POP3Server *server);
LONG DisplayListPOP3Remove(REG(a2) char **array,REG(a1) struct INet_POP3Server *server);
LONG DisplayListEvent(REG(a2) char **array,REG(a1) struct Event *event);
LONG DisplayListEventRemove(REG(a2) char **array,REG(a1) struct Event *event);
LONG DisplayListEventUpdate(REG(a2) char **array,REG(a1) struct Event *event);

void ClearEntry_List(struct List *list);
void UserDataToGUI(UserData *udata);
void GUIToUserData(UserData *udata);
void RedrawEventList();

void GroupAddDate();
void GroupRemDate();
void GroupAddMonth();
void GroupRemMonth();


extern void SaveEvents();

BOOL gui_SendGUIMsg(LONG msgid,APTR data);
extern void ReturnGUIMsg(GUIMsg *msg);
extern void FreeGUIMsg(GUIMsg *msg);
//extern void DoMethods(ObjApp *);
void RemoveEntry_List(struct List *list);
ULONG ConvertString(Object *object);
void Read_Subboard(ItemHeader *header);
extern void InsertMessageHeader(APTR List,char *header, char *text);
extern Date actualdate;

//struct Library * MUIMasterBase=NULL;
struct PortData *mport=NULL;      			                      // internal use
struct ObjApp * App = NULL;                                     // Application object
struct PortData *edit_cport=NULL;
struct UserData *edit_udata=NULL;
ULONG  MaintanceScreenModeID=0;
ULONG  ClientScreenModeID=0;

extern MainPortConfig *mainportconfig;
extern struct MsgPort *GUIPort; // MsgPort of this Programm
extern struct MsgPort *MainGUIPort;                     // MsgPort from MainPort for GUI-Msgs.
extern ULONG  numberofclients;
extern struct List *client_list;
extern struct GUIFrontend guifrontend;

//struct MinList  LocalMailServerList;
struct INet_LocalMailServer	*current_LocalMailServer=NULL;
struct INet_MailUser				*current_MailUser=NULL;
struct Event						*current_event=NULL;
ULONG	LocalMailServers=0;

//struct MinList  POP3ServerList;
struct INet_POP3Server			*current_POP3Server=NULL;
ULONG	POP3Servers=0;

ULONG State=2;
List  dirlist;
List	itemlist;

struct MyNode *currentdir=0;
ULONG dircounter=0;
ULONG itemcounter=0;
char grp_date=FALSE;
char grp_month=FALSE;
char reminder_interval=TRUE;
char datetype_space=FALSE;

char Baseviewstring[150];

char *PStatus(BYTE Status)
{
	switch (Status)
	{
	case 0:
		return "Port is closed";
		break;
	case 1:
		return "Port is offline";
		break;
	case 2:
		return "online, waiting for call";
		break;
	case 3:
		return "online, carrier detected";
		break;
	case 4:
		return "online, user has login";
		break;
	}
	return 0;
}

void gui_CloseIt(char error_code, char *error_text)
{
	ULONG counter;
	UserNode *next;
	BOOL quit=FALSE;
	struct  GUIMsg *msg;
	if (error_code)
		printf("guifrontend error: %s\n",error_text);
	if (edit_udata)FreeVec(edit_udata);
	if (App)DisposeApp(App);
//	if (MUIMasterBase)CloseLibrary(MUIMasterBase);
	if (GUIPort)DeleteMsgPort(GUIPort);
	GUIPort=NULL;
	guifrontend.GUIOpen=FALSE;
   Exit((int)error_code);
}

void gui_main()
{
	BOOL    running = TRUE;
	ULONG   signal;
	ULONG   counter;
	ULONG   clientlistcounter=0;
	BOOL    quit = FALSE;
	struct  GUIMsg *msg;
	ULONG   ulong;
	APTR    old_hide;
	ULONG   result=0;
	struct List_Entry *entry;
	char    IPNumber[4];
	char    *itsme;
	APTR    address;
	char    *buf;
	struct  PortData2 *nc;
	struct  PortData  *cport2=NULL;
	struct  UserNode  *usernode;
	char	  buffer[256];

	/************************************************************************/
	/***                          Open Resources                          ***/
	/************************************************************************/

	NewList(&dirlist);
	NewList(&itemlist);

	GUIPort=CreateMsgPort();
	if (!GUIPort)gui_CloseIt(20,"Can't create GUI-MsgPort");
	GUIPort->mp_Node.ln_Name="NoviaGUI-Frontend_Port";

//	if (!(MUIMasterBase = OpenLibrary(MUIMASTER_NAME,0)))
//		gui_CloseIt(20,"Can't open muimaster.library");

	if (!(MUIMasterBase))
		gui_CloseIt(20,"Can't open muimaster.library");

	if (!(App = CreateApp()))
		gui_CloseIt(20,"Can't create Application");
//	GroupAddDate();
//	GroupAddMonth();
	set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_ShowMe,FALSE);     // show SerialConfig 
	set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_ShowMe,TRUE);     // show SerialConfig 

	set(App->GR_REMINDER_DATETYPE_DATE,MUIA_Disabled,FALSE);
	set(App->STR_REMINDER_ENDDATE,MUIA_Disabled,TRUE);
	set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_Disabled,TRUE);

	grp_date=TRUE;
	grp_month=FALSE;
	

	if (!(edit_udata=AllocVec(sizeof(UserData),MEMF_ANY|MEMF_CLEAR)))
		gui_CloseIt(20,"Not anouth memory");

	/************************************************************************/
	/***               Search for MainPort and get structures             ***/
	/************************************************************************/

	old_hide=App->GR_LOCAL;     					// save actual show group ClientConfig->Network-Config
	set(App->GR_LOCAL,MUIA_ShowMe,TRUE);     // show SerialConfig 

	/************************************************************************/
	/***                              Mainloop                            ***/
	/************************************************************************/

//      LV_Client_List
//      LV_SS_Clients

	Delay(5);
	mport=guifrontend.mport;
	nc=(PortData2 *)client_list->lh_Head;
	while (nc->port_node.ln_Succ)
	{
		DoMethod(App->LV_Client_List, MUIM_List_Insert, &nc->port_address, 1, MUIV_List_Insert_Bottom);
		DoMethod(App->LV_SS_Clients, MUIM_List_Insert, &nc->port_address, 1, MUIV_List_Insert_Bottom);
		nc=(PortData2 *)nc->port_node.ln_Succ;
	}

	for (ulong=1; ulong<mport->UL.UserEntrys; ulong++)
	{
		usernode=&mport->UL.utable[ulong];
	   DoMethod(App->LV_SS_Users, MUIM_List_Insert, &usernode, 1, MUIV_List_Insert_Bottom);
	   DoMethod(App->LV_SS_TCP_MAILBOX_USERS_NAME, MUIM_List_Insert, &usernode, 1, MUIV_List_Insert_Bottom);
	}

	{
		struct Event *event=(Event *)mainportconfig->EventList.mlh_Head;
		while (event->ln_Succ)
		{
			DateToString(buffer,&event->BeginDate,0);
			DoMethod(App->LV_REMINDER, MUIM_List_Insert, &event, 1, MUIV_List_Insert_Bottom);
			event=(Event *)event->ln_Succ;
		}
	}


	{
		struct INet_LocalMailServer *server=(INet_LocalMailServer *)mainportconfig->LocalMailServerList.mlh_Head;
		while (server->ln_Succ)
		{
			DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_Insert, &server, 1, MUIV_List_Insert_Bottom);
			server=(INet_LocalMailServer *)server->ln_Succ;
		}
	}

	{
		struct INet_POP3Server *server=(INet_POP3Server *)mainportconfig->POP3ServerList.mlh_Head;
		while (server->ln_Succ)
		{
			DoMethod(App->LV_SS_TCP_POP_HOSTS, MUIM_List_Insert, &server, 1, MUIV_List_Insert_Bottom);
			server=(INet_POP3Server *)server->ln_Succ;
		}
	}




	DoMethods(App);         // Send Notifications to MUI

	MainPortConfigToGUI(mainportconfig);




	sprintf(buffer,"%sroot",mport->sysdirs->sysdata);
	UpdateBaselist(buffer);

	itemcounter=0;
	dircounter=0;






	while (running && !guifrontend.ProgramClose)
	{
		result=DoMethod(App->App,MUIM_Application_Input,&signal);
		if (result){
#ifdef DEBUG
		printf("resulltID:%d\n",result);
#endif
		switch (result)
		{
		case MUIV_Application_ReturnID_Quit:
			if (MUI_RequestA(App->App, 0, 0,"System Request", "_Ok|_Cancel","Really Shutdown \nNoViA BBS System",0))
				gui_SendGUIMsg(GUIMSG_CLOSE_MAINPORT,0);                        
			break;
		case GUIID_SYSTEM_SERVICE:
			MainPortConfigToGUI(mainportconfig);
			set(App->WindowSystemService,MUIA_Window_Open,TRUE);
			break;
		case GUIID_BASE:
			break;
		case GUIID_FILEBASE:
			break;
		case GUIID_CHAT_CHANNELS:
			break;
		case GUIID_NEWS:
			break;
		case GUIID_PFILES:
/*                      set(App->GR_WindowClientList,MUIA_Disabled,TRUE);               // first, hide all other windows
			set(App->GR_WindowClientConfig,MUIA_Disabled,TRUE);
			set(App->GR_WindowSystemService,MUIA_Disabled,TRUE);
			set(App->WindowRegistration,MUIA_Window_Open,TRUE);
*/
			break;
		case GUIID_VOTES:
			break;
		case GUIID_AUSLASTUNG:
			break;
		case GUIID_GFILES:
			break;
		case GUIID_CLIENT_LIST:                 
			break;
		case GUIID_USERINFO:
			break;
		case GUIID_PORTINFO:
			break;
		case GUIID_MAILBOX:
			break;
		case GUIID_CH_CC_loadatstart:
			break;
		case GUIID_SELECT_ALL:
			counter=TRUE;
		    DoMethod(App->LV_Client_List, MUIM_List_Select,
				MUIV_List_Select_All, MUIV_List_Select_On,&counter);
			break;
		case GUIID_SELECT_NOTHING:
			counter=TRUE;
		    DoMethod(App->LV_Client_List, MUIM_List_Select,
				MUIV_List_Select_All, MUIV_List_Select_Off,&counter);
			break;
		case GUIID_CONFIG:
			break;
		case GUIID_ONLINE:
			ulong = MUIV_List_NextSelected_Start;
			for (;;)
			{
				DoMethod(App->LV_Client_List, MUIM_List_NextSelected, &ulong);
				if (ulong == MUIV_List_NextSelected_End) break;
				DoMethod(App->LV_Client_List, MUIM_List_GetEntry, ulong, &cport2);
				gui_SendGUIMsg(GUIMSG_CLIENT_ONLINE,cport2);                                    
			}
			break;
		case GUIID_OFFLINE:
		ulong = MUIV_List_NextSelected_Start;
			for (;;)
			{
				DoMethod(App->LV_Client_List, MUIM_List_NextSelected, &ulong);
				if (ulong == MUIV_List_NextSelected_End) break;
				DoMethod(App->LV_Client_List, MUIM_List_GetEntry, ulong, &cport2);
				gui_SendGUIMsg(GUIMSG_CLIENT_OFFLINE,cport2);
			}
			break;
		case GUIID_REMOVE:
		ulong = MUIV_List_NextSelected_Start;
			for (;;)
			{
				DoMethod(App->LV_Client_List, MUIM_List_NextSelected, &ulong);
				if (ulong == MUIV_List_NextSelected_End) break;
				DoMethod(App->LV_Client_List, MUIM_List_GetEntry, ulong, &cport2);
				gui_SendGUIMsg(GUIMSG_CLIENT_UNLOAD,cport2);
			}
			break;
		case GUIID_LOAD:
			break;
		case GUIID_ABOUT:
		MUI_RequestA(App->App, 0, 0,"System Request","_Ok",
			"NOVIA, Boud Bandit Software. V 0.00.06alpha \nCopyright ©1996-1999 by Thorsten Gehler/UFoP\nFrom: "__DATE__" "__TIME__,0);
			break;
		case GUIID_ABOUTMUI:
			set(App->ABOUT,MUIA_Window_Open,TRUE);
			break;
		case GUIID_LOCKCONSULE:
			set(App->GR_WindowClientList,MUIA_Disabled,TRUE);               // first, hide all other windows
			set(App->GR_WindowClientConfig,MUIA_Disabled,TRUE);
			set(App->GR_WindowSystemService,MUIA_Disabled,TRUE);
			set(App->WindowLocked,MUIA_Window_Open,TRUE);
			break;
		case GUIID_RESTARTSERVER:
			break;
		case GUIID_RESETSERVER:
			break;
		case GUIID_QUIT:
		if (MUI_RequestA(App->App, 0, 0,"System Request", "_Ok|_Cancel","Really Shutdown \nNoViA BBS System",0))
			gui_SendGUIMsg(GUIMSG_CLOSE_MAINPORT,0);                        
			break;
		case GUIID_LOCALLOGIN:
			break;
		case GUIID_NONEWUSERS:
			break;
		case GUIID_BUSYALLPORTS:
			break;
		case GUIID_DISABLEBASE:
			break;
		case GUIID_CONFIGBASE:
			break;
		case GUIID_DISABLEUDBASE:
			break;
		case GUIID_CONFIGUDBASE:
			break;
		case GUIID_CHATCHANNELS:
			break;
		case GUIID_DISABLECC:
			break;
		case GUIID_DISABLENEWS:
			break;
		case GUIID_CONFIGNEWS:
			break;
		case GUIID_DISABLEPFILES:
			break;
		case GUIID_CONFIGPFILES:
			break;
		case GUIID_DISABLEGFILES:
			break;
		case GUIID_CONFIGGFILES:
			break;
		case GUIID_DISABLEVOTES:
			break;
		case GUIID_CONFIGCONFIG:
			break;
		case GUIID_BT_CC_Use:
			GUIToClientConfig(&edit_cport->clientconfig);
			gui_SendGUIMsg(GUIMSG_CLIENT_USE,edit_cport);
			set(App->WindowClientConfig,MUIA_Window_Open,FALSE);
			break;
		case GUIID_BT_CC_Save:
			GUIToClientConfig(&edit_cport->clientconfig);
			gui_SendGUIMsg(GUIMSG_CLIENT_SAVE,edit_cport);
			set(App->WindowClientConfig,MUIA_Window_Open,FALSE);
			break;
		case GUIID_BT_CC_Cancel:
			gui_SendGUIMsg(GUIMSG_CLIENT_CANCEL,mport);
			set(App->WindowClientConfig,MUIA_Window_Open,FALSE);
			break;

		case GUIID_DC_CLIENT_LIST:
			ulong = MUIV_List_NextSelected_Start;
			DoMethod(App->LV_Client_List, MUIM_List_NextSelected, &ulong);
			if (ulong!=MUIV_List_NextSelected_End)
			{
				DoMethod(App->LV_Client_List, MUIM_List_GetEntry, ulong, &cport2);
				if (cport2->PortStatus==PORT_STATUS_CLOSE)
				{
#ifdef DEBUG
					printf("online");
#endif
					gui_SendGUIMsg(GUIMSG_CLIENT_ONLINE,cport2);
				}
				else
				{
#ifdef DEBUG
					printf("tofront");
#endif
					gui_SendGUIMsg(GUIMSG_CLIENT_TOFRONT,cport2);                                   
				}
			}
			break;
		case GUIID_DC_SS_CLIENT_LIST:
			ulong = MUIV_List_NextSelected_Start;
			DoMethod(App->LV_SS_Clients, MUIM_List_NextSelected, &ulong);
			if (ulong!=MUIV_List_NextSelected_End)
			{
				DoMethod(App->LV_SS_Clients, MUIM_List_GetEntry, ulong, &edit_cport);
				ClientConfigToGUI(&edit_cport->clientconfig);
				set(App->WindowClientConfig,MUIA_Window_Open,TRUE);
			}
			break;
		case GUIID_DC_SS_USERS:
			ulong = MUIV_List_NextSelected_Start;
			DoMethod(App->LV_SS_Users, MUIM_List_NextSelected, &ulong);
			if (ulong!=MUIV_List_NextSelected_End)
			{
				struct UserNode un;
				usernode=&un;
				DoMethod(App->LV_SS_Users, MUIM_List_GetEntry, ulong, &usernode);
				if (edit_udata->IDNumber)
				{
					GUIToUserData(edit_udata);
					SaveUser(edit_udata);
				}
				if (LoadUser(edit_udata,usernode->IDNumber))
				{
					UserDataToGUI(edit_udata);
					set(App->WindowUserEditor,MUIA_Window_Open,TRUE);
				}
			}
			break;
		case GUIID_USER_CLOSEWIN:
			if (edit_udata->IDNumber)
			{
				if (edit_udata->IDNumber)
				{
					GUIToUserData(edit_udata);
					SaveUser(edit_udata);
				}
				edit_udata->IDNumber=0;
				set(App->WindowUserEditor,MUIA_Window_Open,FALSE);
			}
			break;

		/************************************************************************/
		/***                                                  STRING - CASING ***/
		/************************************************************************/


		case GUIID_SS_CLIENTS_NEW:
			gui_SendGUIMsg(GUIMSG_CLIENT_NEW,0);
			break;
		case GUIID_SS_CLIENTS_EDIT:
		ulong = MUIV_List_NextSelected_Start;
			DoMethod(App->LV_SS_Clients, MUIM_List_NextSelected, &edit_cport);
			if (ulong!=MUIV_List_NextSelected_End)
			{
				DoMethod(App->LV_SS_Clients, MUIM_List_GetEntry, ulong, &edit_cport);
				ClientConfigToGUI(&edit_cport->clientconfig);
				set(App->WindowClientConfig,MUIA_Window_Open,TRUE);
			}
			gui_SendGUIMsg(GUIMSG_CLIENT_EDIT,0);
			break;
		case GUIID_SS_CLIENTS_REMOVE:
			break;
		case GUIID_SS_USERS_NEW:
			break;
		case GUIID_SS_USERS_EDIT:
			break;
		case GUIID_SS_USERS_REMOVE:
			break;

		case GUIID_SS_ACCEPT:
			{
				struct INet_LocalMailServer *mailserver=NULL;
				gui_SendGUIMsg(GUIMSG_MPCONFIG_SAVE,0);

				GUIToMainPortConfig(mainportconfig);
//				GUIToClientConfig();

				if (current_LocalMailServer)
					GUIToINet_LocalMailServer(current_LocalMailServer);
				if (current_MailUser)
					GUIToINet_LocalUser(current_MailUser);
				if (current_POP3Server)
					GUIToINet_POP3Server(current_POP3Server);
				if (current_event)
					GUIToEvent(current_event);

//				GUIToUserData(UserData *udata);
				
				set(App->WindowSystemService,MUIA_Window_Open,FALSE);
				SaveEvents();
				SaveMailServers();
			}
			break;
		case GUIID_SS_CANCEL:
			set(App->WindowSystemService,MUIA_Window_Open,FALSE);
			break;


		case GUIID_CL_OK:
			set(App->GR_WindowClientList,MUIA_Disabled,FALSE);              // first, hide all other windows
			set(App->GR_WindowClientConfig,MUIA_Disabled,FALSE);
			set(App->GR_WindowSystemService,MUIA_Disabled,FALSE);
			set(App->WindowLocked,MUIA_Window_Open,FALSE);
			break;
		case GUIID_CL_CANCEL:
			set(App->GR_WindowClientList,MUIA_Disabled,FALSE);              // first, hide all other windows
			set(App->GR_WindowClientConfig,MUIA_Disabled,FALSE);
			set(App->GR_WindowSystemService,MUIA_Disabled,FALSE);
			set(App->WindowLocked,MUIA_Window_Open,FALSE);
			break;
		case GUIID_NEWREGISTER:                                                 // Get new Registration-input
			set(App->WindowClientList,MUIA_Disabled,TRUE);          // first, hide all other windows
			set(App->WindowClientConfig,MUIA_Disabled,TRUE);
			set(App->WindowSystemService,MUIA_Disabled,TRUE);
			set(App->WindowRegistration,MUIA_Window_Open,TRUE);
			Delay(200);
			set(App->WindowClientList,MUIA_Disabled,FALSE);         // unhide all windows
			set(App->WindowClientConfig,MUIA_Disabled,FALSE);
			set(App->WindowSystemService,MUIA_Disabled,FALSE);
			break;
		case GUIID_SCREENMODE:
			break;
		case GUIID_CY_NETWORKCON:
			get(App->CY_CC_NETWORKTYPE,MUIA_Cycle_Active,&ulong); // get actual cycle pos.
			set(old_hide,MUIA_ShowMe,FALSE);                                                 // hide old group

			switch (ulong)                         /*** old_hide = new show group ***/
			{
				case FD_MUI_SCONFIG_ADDRESS_LOCAL:
					old_hide=App->GR_LOCAL;
					break;					
				case FD_MUI_SCONFIG_ADDRESS_SERIAL:
					old_hide=App->GR_SerialConfig;
					break;
				case FD_MUI_SCONFIG_ADDRESS_PARALLEL:
					old_hide=App->GR_Parallel;
					break;
				case FD_MUI_SCONFIG_ADDRESS_TCPIP:
					old_hide=App->GR_TCPIP;
					break;
				case FD_MUI_SCONFIG_ADDRESS_ENVOY:
					old_hide=App->GR_ENVOY;
					break;
				case FD_MUI_SCONFIG_ADDRESS_NOVELL:
					old_hide=App->GR_IPX;
					break;
				case FD_MUI_SCONFIG_ADDRESS_MSN:
					old_hide=App->GR_MSN;
					break;
			}
			set(old_hide,MUIA_ShowMe,TRUE);         // show new group
		case GUIID_SS_TCP_MAILBOX_NEW:
			{
				struct INet_LocalMailServer *newserver;
				if ((newserver=AllocVec(sizeof(INet_LocalMailServer),MEMF_ANY|MEMF_CLEAR)))
				{
					strcpy(newserver->Name,"NewServer");
					newserver->Status=SERVER_ADDED;
					LocalMailServers++;
					AddTail((List *)&mainportconfig->LocalMailServerList,(Node *)newserver);
					NewList((List *)&newserver->Users);
					DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_Insert, &newserver, 1, MUIV_List_Insert_Bottom);
					set(App->LV_SS_TCP_MAILBOX_HOSTS,MUIA_List_Active, MUIV_List_Active_Bottom);
#ifdef DEBUG
					printf("newserver: %x Empty: %d\n",newserver,IsListEmpty((List*)&newserver->Users));
#endif
				}
			}
			break;
		case GUIID_SS_TCP_MAILBOX_REMOVE:
			if (current_LocalMailServer)
			{
				ULONG entries;
				DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_Remove,MUIV_List_Remove_Active);
				current_LocalMailServer->Status=SERVER_REMOVED;
				get(App->LV_SS_TCP_MAILBOX_HOSTS,MUIA_List_Entries,&entries);
				if (!entries)
				{
					current_LocalMailServer=NULL;
					current_MailUser=NULL;
					set(App->GR_SS_TCP_MAILBOX_PARAMETERS,MUIA_Disabled,TRUE);
				}
			}
			break;
		case GUIID_SS_TCP_MAILBOX_NAME:
			if (current_LocalMailServer)
			{
				GetX(App->STR_SS_TCP_MAILBOX_NAME,current_LocalMailServer->Name,40);
				DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_Redraw, MUIV_List_Redraw_Active);
			}
			break;
		case GUIID_SS_TCP_MAILBOX_USER_NEW:
			if (current_LocalMailServer)
			{
				struct INet_MailUser *newuser;
				if ((newuser=AllocVec(sizeof(INet_MailUser),MEMF_ANY|MEMF_CLEAR)))
				{
#ifdef DEBUG
					printf("newuser: 0x%x\n",newuser);
#endif
//					LocalMailServers++;
					AddTail((List *)&current_LocalMailServer->Users,(Node *)newuser);
					DoMethod(App->LV_SS_TCP_MAILBOX_USERS, MUIM_List_Insert, &newuser, 1, MUIV_List_Insert_Bottom);
					set(App->LV_SS_TCP_MAILBOX_USERS,MUIA_List_Active, MUIV_List_Active_Bottom);
				}
			}
			break;
		case GUIID_SS_TCP_MAILBOX_USER_REMOVE:
			if (current_MailUser)
			{
				ULONG entries;
				DoMethod(App->LV_SS_TCP_MAILBOX_USERS, MUIM_List_Remove,MUIV_List_Remove_Active);
				current_MailUser->Status=USER_REMOVED;
				get(App->LV_SS_TCP_MAILBOX_USERS,MUIA_List_Entries,&entries);
				if (!entries)
				{
					current_MailUser=NULL;
					set(App->GR_SS_TCP_MAILBOX_USERS_PARAMETERS,MUIA_Disabled,TRUE);
				}
			}
			break;
		case GUIID_SS_TCP_MAILBOX_USER_CHANGENAME:
#ifdef DEBUG
			printf("change username\n");
#endif
			if (current_MailUser)
			{
				GetX(App->STR_SS_TCP_MAILBOX_NAME,current_MailUser->Name,40);
				DoMethod(App->LV_SS_TCP_MAILBOX_USERS,MUIM_List_Redraw, MUIV_List_Redraw_Active);
			}		
			break;
		case GUIID_SS_TCP_MAILBOX_USERSELECT_DC:
			if (current_MailUser)
			{
				struct UserNode *udata=NULL;
				ULONG ulong = MUIV_List_GetEntry_Active;
//				DoMethod(App->LV_SS_TCP_MAILBOX_USERS_NAME, MUIM_List_NextSelected, &ulong);
#ifdef DEBUG
				printf("u1\n");
#endif
//				if (ulong!=MUIV_List_NextSelected_End)
				{
					DoMethod(App->LV_SS_TCP_MAILBOX_USERS_NAME, MUIM_List_GetEntry, ulong, &udata);
#ifdef DEBUG
					printf("u2\n");
#endif
					if (udata)
					{
						strncpy(current_MailUser->Name,udata->Handle,21);
						set(App->STR_PO_SS_TCP_MAILBOX_USERS_NAME,MUIA_String_Contents,udata->Handle);
//						DoMethod(App->LV_SS_TCP_MAILBOX_USERS,MUIM_List_Redraw, MUIV_List_Redraw_Active);
						set(App->GR_SS_TCP_MAILBOX_USERS_NAME,MUIA_ShowMe,FALSE);
//						DoMethod(App->GR_SS_TCP_MAILBOX_USERS_NAME, MUIM_Hide);
					}
				}
			}

			break;
		case GUIID_SS_TCP_MAILBOX_CHANGEHOST:
			if (current_LocalMailServer)
			{
				GetX(App->STR_SS_TCP_MAILBOX_NAME,current_LocalMailServer->Name,40);
				DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS,MUIM_List_Redraw, MUIV_List_Redraw_Active);
			}
			break;
		case GUIID_SS_TCP_POP3_NEW:
			{
				struct INet_POP3Server *newserver;
				if ((newserver=AllocVec(sizeof(INet_POP3Server),MEMF_ANY|MEMF_CLEAR)))
				{
					strcpy(newserver->Name,"NewServer");
					newserver->Status=SERVER_ADDED;
					POP3Servers++;
					AddTail((List *)&mainportconfig->POP3ServerList,(Node *)newserver);
					DoMethod(App->LV_SS_TCP_POP_HOSTS, MUIM_List_Insert, &newserver, 1, MUIV_List_Insert_Bottom);
					set(App->GR_SS_TCP_POP_PARA,MUIA_Disabled,FALSE);
					set(App->LV_SS_TCP_POP_HOSTS,MUIA_List_Active, MUIV_List_Active_Bottom);
				}
			}
			break;
		case GUIID_SS_TCP_POP3_REMOVE:
			if (current_POP3Server)
			{
				ULONG entries;
				DoMethod(App->LV_SS_TCP_POP_HOSTS, MUIM_List_Remove,MUIV_List_Remove_Active);
				current_POP3Server->Status=SERVER_REMOVED;
				get(App->LV_SS_TCP_POP_HOSTS,MUIA_List_Entries,&entries);
				if (!entries)
				{
					current_POP3Server=NULL;
					set(App->GR_SS_TCP_POP_PARA,MUIA_Disabled,TRUE);
				}
			}
			break;
		case GUIID_SS_TCP_POP3_CHANGEHOST:
			if (current_POP3Server)
			{
				GetX(App->STR_SS_TCP_POP_NAME,current_POP3Server->Name,40);
				DoMethod(App->LV_SS_TCP_POP_HOSTS,MUIM_List_Redraw, MUIV_List_Redraw_Active);
			}
			break;
		case GUIID_SS_BASE_OPEN: case GUIID_SS_BASE_SELECT:
			{
				struct ItemHeader *header;
				ulong = MUIV_List_NextSelected_Start;
				for (;;)
				{
					DoMethod(App->LV_SS_BASEVIEW, MUIM_List_NextSelected, &ulong);
					if (ulong == MUIV_List_NextSelected_End) break;
					DoMethod(App->LV_SS_BASEVIEW, MUIM_List_GetEntry, ulong, &header);
				}
				if (header)
				{
					if (header->ItemType==ITEMTYPE_SUBBOARD)
					{
						if (header->Path)
						{
							strcpy(buffer,header->Path);
							DoMethod(App->LV_SS_BASEVIEW, MUIM_List_Clear);
							UpdateBaselist(buffer);
						}
					}
					else
					{
						if (header->ItemType==ITEMTYPE_ITEM)
							Read_Subboard(header);
					}
				}
			}
			break;
		case GUIID_SS_BASE_EDIT:
			break;
		case GUIID_SS_BASE_NEW:
			break;
		case GUIID_SS_BASE_REMOVE:
			break;
		case GUIID_SS_BASE_PARENT:
			ParrentBaselist();
			break;
		case GUIID_REMINDER_NEW:
			{
				struct Event *newevent;
				if ((newevent=AllocVec(sizeof(Event),MEMF_ANY|MEMF_CLEAR)))
				{
					GetTime(&actualdate);
					strcpy(newevent->Name,"NewEvent");
					CopyMemQuick(&actualdate,&newevent->BeginDate,sizeof(Date));
					CopyMemQuick(&actualdate,&newevent->EndDate,sizeof(Date));
					newevent->Multiplicator=MULTIPLICATOR_MINS;
					newevent->IntervalTime=10;
					AddTail((List *)&mainportconfig->EventList,(Node *)newevent);
					DoMethod(App->LV_REMINDER, MUIM_List_Insert, &newevent, 1, MUIV_List_Insert_Bottom);
					set(App->LV_REMINDER,MUIA_List_Active, MUIV_List_Active_Bottom);
				}
			}
			break;
		case GUIID_REMINDER_COPY:
			{
				struct Event *newevent;
				if ((newevent=AllocVec(sizeof(Event),MEMF_ANY|MEMF_CLEAR)))
				{
					CopyMemQuick(current_event,newevent,sizeof(Event));
					ClearMemQuick(&newevent->LastEvent,sizeof(Date));
					ClearMemQuick(&newevent->NextEvent,sizeof(Date));
					newevent->Running=FALSE;
					newevent->init=FALSE;
					AddTail((List *)&mainportconfig->EventList,(Node *)newevent);
					DoMethod(App->LV_REMINDER, MUIM_List_Insert, &newevent, 1, MUIV_List_Insert_Bottom);
					set(App->LV_REMINDER,MUIA_List_Active, MUIV_List_Active_Bottom);
				}
			}
			break;
		case GUIID_REMINDER_ADD:
			break;
		case GUIID_REMINDER_TEST:
			if (current_event)
				ExecuteEvent(current_event);
			break;
		case GUIID_REMINDER_UPDATE:
			break;
		case GUIID_REMINDER_REMOVE:
			if (current_event)
			{
				ULONG entries;
				DoMethod(App->LV_REMINDER, MUIM_List_Remove,MUIV_List_Remove_Active);
				get(App->LV_REMINDER,MUIA_List_Entries,&entries);
				if (!entries)
					current_event=NULL;
			}
			break;
		case GUIID_REMINDER_NAME:
			if (current_event)
			{
				GetX(App->STR_REMINDER,current_event->Name,20);
				DoMethod(App->LV_REMINDER, MUIM_List_Redraw, MUIV_List_Redraw_Active);
			}
			break;
		case GUIID_REMINDER_INTERVALTYPE:
			get(App->CY_REMINDER_INTERVALTYPE,MUIA_Cycle_Active,&ulong);
			switch (ulong)
			{
				case 0:
					if (!reminder_interval)
					{
						set(App->STR_REMINDER_ENDTIME,MUIA_Disabled,FALSE);
						set(App->STR_REMINDER_INTERVAL,MUIA_Disabled,FALSE);
						reminder_interval=TRUE;
					}
					break;
				case 1:
					if (reminder_interval)
					{
						set(App->STR_REMINDER_ENDTIME,MUIA_Disabled,TRUE);
						set(App->STR_REMINDER_INTERVAL,MUIA_Disabled,TRUE);
						reminder_interval=FALSE;
					}
					break;
			}
			break;
		case GUIID_REMINDER_HOLIDAYS:
			get(App->CY_REMINDER_HOLIDAYS,MUIA_Cycle_Active,&ulong);
			switch (ulong)
			{
				case 0:
					break;
				case 1:
					break;
			}
			break;
			break;
		case GUIID_REMINDER_DATETYPE:
			get(App->CY_REMINDER_DATETYPE,MUIA_Cycle_Active,&ulong);
			switch (ulong)
			{
				case 0:			// Date
					GroupRemMonth();
					GroupAddDate();
					if (datetype_space)
					{
						set(App->STR_REMINDER_ENDDATE,MUIA_Disabled,TRUE);
						datetype_space=FALSE;
					}
					break;
				case 1:			// Space
					GroupRemMonth();
					GroupAddDate();
					if (!datetype_space)
					{
						set(App->STR_REMINDER_ENDDATE,MUIA_Disabled,FALSE);
						datetype_space=TRUE;
					}
					break;
				case 2:			// Month
					GroupRemDate();
					GroupAddMonth();
					break;
				case 3:			// Daily
					GroupRemDate();
					GroupRemMonth();
					break;
			}
			break;
			break;
		}}


		/***************************************/
		/***  MESSAGES                       ***/
		/***************************************/

		if (running && signal && !msg) Wait(signal|(1<<GUIPort->mp_SigBit));    // Waiting of Signals
		msg=(GUIMsg *)GetMsg(GUIPort);
		if (msg)
		{
			switch (msg->cmdid)
			{
			case GUIMSG_REPLY:                                      // Std-Reply
				FreeGUIMsg(msg);
				break;
			case GUIMSG_CLOSE_FRONT_END:            // Normal Programmend
				ReturnGUIMsg(msg);
				running = FALSE;
				break;
			case GUIMSG_CLOSE_FAIL20:                       // Main Port failat. CloseIt NOW!!!
				ReturnGUIMsg(msg);
				running = FALSE;
				break;
			case GUIMSG_CLIENT_ADD:                         // Add Client, from MainPort
				if (msg->data)
				{
				    DoMethod(App->LV_Client_List, MUIM_List_Insert, &msg->data, 1, MUIV_List_Insert_Bottom);
				    DoMethod(App->LV_SS_Clients, MUIM_List_Insert, &msg->data, 1, MUIV_List_Insert_Bottom);
				}
				ReturnGUIMsg(msg);
				break;
			case GUIMSG_CLIENT_REMOVE:
				quit=FALSE;
				for (counter=0;(!quit && counter<numberofclients); counter++)
				{
					DoMethod(App->LV_Client_List, MUIM_List_GetEntry, counter, &cport2);
					if (mport==(PortData *)msg->data)quit=TRUE;
				}
				if (quit && cport2)
				{
				    DoMethod(App->LV_Client_List, MUIM_List_Remove, counter);
				    DoMethod(App->LV_SS_Clients, MUIM_List_Remove, counter);
				}
				ReturnGUIMsg(msg);
				break;  
			case GUIMSG_CLIENT_CHANGE:
				quit=FALSE;
				for (counter=0;(!quit && counter<numberofclients); counter++)
				{
					DoMethod(App->LV_Client_List, MUIM_List_GetEntry, counter, &cport2);
					if (mport==(PortData *)msg->data)quit=TRUE;
				}
				if (quit && cport2)
				{
					DoMethod(App->LV_Client_List, MUIM_List_Redraw, counter);
					DoMethod(App->LV_SS_Clients, MUIM_List_Redraw, counter);
				}
				ReturnGUIMsg(msg);
				break;
			case GUIMSG_CLIENTLIST_REFRESH:
				nc=(PortData2 *)client_list->lh_Head;
				while (nc->port_node.ln_Succ)
				{
					DoMethod(App->LV_Client_List, MUIM_List_Insert, &nc->port_address, 1, MUIV_List_Insert_Bottom);
					DoMethod(App->LV_SS_Clients, MUIM_List_Insert, &nc->port_address, 1, MUIV_List_Insert_Bottom);
					nc=(PortData2 *)nc->port_node.ln_Succ;
				}
				ReturnGUIMsg(msg);
				break;
			case GUIMSG_CLIENT_EDIT:
				edit_cport=msg->data;
				ReturnGUIMsg(msg);
				ClientConfigToGUI(&edit_cport->clientconfig);
				set(App->WindowClientConfig,MUIA_Window_Open,TRUE);
				break;
			case GUIMSG_ABOUT:
				ReturnGUIMsg(msg);
			MUI_RequestA(App->App, 0, 0,"System Request","_Ok",
				"NOVIA, Boud Bandit Software. V 0.00.06alpha \nCopyright ©1996-1999 by Thorsten Gehler/UFoP\nFrom: "__DATE__" "__TIME__,0);
				break;
			case GUIMSG_QUIT:
				ReturnGUIMsg(msg);
			if (MUI_RequestA(App->App, 0, 0,"System Request", "_Ok|_Cancel","Really Shutdown \nNoViA BBS System",0))
				gui_SendGUIMsg(GUIMSG_CLOSE_MAINPORT,0);                        
				break;
			case GUIMSG_LOCKCONSULE:
				ReturnGUIMsg(msg);
				set(App->GR_WindowClientList,MUIA_Disabled,TRUE);               // first, hide all other windows
				set(App->GR_WindowClientConfig,MUIA_Disabled,TRUE);
				set(App->GR_WindowSystemService,MUIA_Disabled,TRUE);
				set(App->WindowLocked,MUIA_Window_Open,TRUE);
				break;
			case GUIMSG_REDRAW_REMINDERLIST:
				DoMethod(App->LV_REMINDER, MUIM_NList_Redraw, MUIV_NList_Redraw_All);
				ReturnGUIMsg(msg);
				break;
			}
		}
	 }
	gui_CloseIt(0,"");
}

void RemoveEntry_List(struct List *list)
{
	ClearEntry_List(list);
	FreeVec(list);
}

void ClearEntry_List(struct List *list)
{
	Node *nc;
	if (!list)return;
	nc=list->lh_Head;
	while(!IsListEmpty(list))
	{
		Remove(nc);
		FreeVec(nc);
		nc=list->lh_Head;
	}
	NewList(list);
}

void RedrawEventList()
{
	DoMethod(App->LV_REMINDER, MUIM_NList_Redraw, MUIV_NList_Redraw_All);
}

#include "gui/novia_gui_hooks.c"
#include "gui/novia_gui_helpfuncs.c"



void GroupAddDate()
{
/*	if (!App->GR_REMINDER_DATETYPE_DATE)
	{
		App->GR_REMINDER_DATETYPE_DATE = GroupObject,
//			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Horiz, TRUE,
			Child, Label2("beginn"),
			Child, App->STR_REMINDER_BEGINDATE = StringObject,
				MUIA_Frame, MUIV_Frame_String,
				MUIA_String_MaxLen, 16,
			End,
			Child, Label2("end"),
			Child, App->STR_REMINDER_ENDDATE = StringObject,
				MUIA_Frame, MUIV_Frame_String,
				MUIA_String_MaxLen, 16,
			End,
			Child, HVSpace,
		End;
		if (App->GR_REMINDER_DATETYPE_DATE)
		{
			DoMethod(App->GR_grp_95, OM_ADDMEMBER, App->GR_REMINDER_DATETYPE_DATE);
//			DoMethod(App->GR_grp_95, OM_UPDATE, App->GR_REMINDER_DATETYPE_DATE);
			MUI_Redraw(App->App,MADF_DRAWOBJECT);
//			set(App->GR_REMINDER_DATETYPE_DATE,MUIA_ShowMe,FALSE);     // show SerialConfig 
//			set(App->GR_REMINDER_DATETYPE_DATE,MUIA_ShowMe,TRUE);     // show SerialConfig 
		}
	}
*/
	if (!grp_date)
	{
		set(App->GR_REMINDER_DATETYPE_DATE,MUIA_Disabled,FALSE);
		grp_date=TRUE;
	}
}

void GroupRemDate()
{
/*	if (App->GR_REMINDER_DATETYPE_DATE)
	{
		DoMethod(App->GR_grp_95, OM_REMMEMBER, App->GR_REMINDER_DATETYPE_DATE);
//		MUI_DisposeObject(App->GR_REMINDER_DATETYPE_DATE);
		App->GR_REMINDER_DATETYPE_DATE=NULL;
	}*/
	if (grp_date)
	{
		set(App->GR_REMINDER_DATETYPE_DATE,MUIA_Disabled,TRUE);
		grp_date=FALSE;
	}
}

void GroupAddMonth()
{
/*
	if (!App->GR_REMINDER_DATETYPE_MONTHLY)
	{
		App->GR_REMINDER_DATETYPE_MONTHLY = GroupObject,
//			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Horiz, TRUE,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("1st"),
				Child, App->CH_REMINDER1 = CheckMark(FALSE),
			End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("2nd"),
				Child, App->CH_REMINDER2 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("3rd"),
				Child, App->CH_REMINDER3 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("4th"),
				Child, App->CH_REMINDER4 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("5th"),
				Child, App->CH_REMINDER5 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("6th"),
				Child, App->CH_REMINDER6 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("7th"),
				Child, App->CH_REMINDER7 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("8th"),
				Child, App->CH_REMINDER8 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("9th"),
				Child, App->CH_REMINDER9 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("10th"),
				Child, App->CH_REMINDER10 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("11th"),
				Child, App->CH_REMINDER11 = CheckMark(FALSE),
				End,
			Child, GroupObject,
				MUIA_Group_Columns, 2,
				Child, Label2("12th"),
				Child, App->CH_REMINDER12 = CheckMark(FALSE),
				End,
			Child, HVSpace,
		End;
		if (App->GR_REMINDER_DATETYPE_MONTHLY)
		{
			DoMethod(App->GR_grp_95, OM_ADDMEMBER, App->GR_REMINDER_DATETYPE_MONTHLY);
//			DoMethod(App->GR_grp_95, OM_UPDATE, App->GR_REMINDER_DATETYPE_MONTHLY);
//			MUI_Redraw(App->GR_REMINDER_DATETYPE_MONTHLY,MADF_DRAWOBJECT);
//			set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_ShowMe,FALSE);     // show SerialConfig 
//			set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_ShowMe,TRUE);     // show SerialConfig 
		}
	}*/
	if (!grp_month)
	{
		set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_Disabled,FALSE);
		grp_month=TRUE;
	}
}



void GroupRemMonth()
{
/*	if (App->GR_REMINDER_DATETYPE_MONTHLY)
	{
		DoMethod(App->GR_grp_95, OM_REMMEMBER, App->GR_REMINDER_DATETYPE_MONTHLY);
//		MUI_DisposeObject(App->GR_REMINDER_DATETYPE_MONTHLY);
		App->GR_REMINDER_DATETYPE_MONTHLY=NULL;
	}*/
	if (grp_month)
	{
		set(App->GR_REMINDER_DATETYPE_MONTHLY,MUIA_Disabled,TRUE);
		grp_month=FALSE;
	}
}
