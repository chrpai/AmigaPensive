#include <libraries/mui.h>
#include <libraries/gadtools.h>
#include <clib/muimaster_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/asl_protos.h>
//#include <clib/icon_protos.h>
//#include <clib/graphics_protos.h>
//#include <clib/intuition_protos.h>
//#include <clib/gadtools_protos.h>
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
#include <exec/memory.h>
//#include <libraries/iffparse.h>
#include <dos/stdio.h>

#include "/novia_gui_data.h"
#include "/novia_baseview_data.h"

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

//#include "mcpp:lib/mui.c"
#define REG(x) register __ ## x

//#define ASM
//#define SAVEDS

void GetX(Object *object,APTR pointer,ULONG ulong);
BOOL MainPortConfigToGUI(MainPortConfig *config);
BOOL GUIToMainPortConfig(MainPortConfig *config);
BOOL ClientConfigToGUI(ClientConfig *config);
void GUIToClientConfig(ClientConfig *config);
void INet_POP3ServerToGUI(INet_POP3Server *server);
void INet_LocalMailServerToGUI(INet_LocalMailServer *server);
void GUIToINet_POP3Server(INet_POP3Server *server);
void GUIToINet_LocalMailServer(INet_LocalMailServer *user);
void INet_LocalUserToGUI(INet_MailUser *user);
void GUIToINet_LocalUser(INet_MailUser *user);
void EventToGUI(struct Event *event);
void GUIToEvent(struct Event *event);
void UserDataToGUI(UserData *udata);
void GUIToUserData(UserData *udata);
ULONG ConvertString(Object *object);

extern struct Library * MUIMasterBase;
extern struct PortData *mport;      			                      // internal use
extern struct ObjApp * App;                                     // Application object

extern ULONG  MaintanceScreenModeID;
extern ULONG  ClientScreenModeID;
extern struct GUIFrontend guifrontend;
extern struct MsgPort *GUIPort;
extern struct MsgPort *MainGUIPort;                     // MsgPort from MainPort for GUI-Msgs.


/*****************************************************************************
***                                                      Load & Save Funcs ***
*****************************************************************************/

BOOL MainPortConfigToGUI(MainPortConfig *config)
{
	char buffer[200];
	NameInfo *nameinfo=AllocVec(sizeof(NameInfo),MEMF_ANY);

	/************************************/
	/*** SYSTEM SERVICE --> MAINTANCE ***/
	/************************************/

	set(App->STR_SystemName,		MUIA_String_Contents,(APTR)config->SystemName);
	set(App->TX_RegistrationID,	MUIA_String_Contents,(APTR)config->RegID);
	set(App->TX_RegistrationName,	MUIA_String_Contents,(APTR)config->RegName);
	set(App->STR_RegLocation,		MUIA_String_Contents,(APTR)config->RegLocation);
	set(App->STR_MainPortNumber,	MUIA_String_Contents,(APTR)config->MainPortNumber);
	set(App->STR_UUCPDomain,		MUIA_String_Contents,(APTR)config->UUCPDomain);

	set(App->STR_PA_SS_MAINPATH,	MUIA_String_Contents,(APTR)config->sysdirs.sysdir);
	set(App->STR_PA_SS_USERDIR,	MUIA_String_Contents,(APTR)config->sysdirs.userdir);
	set(App->STR_PA_SS_SYSDATA,	MUIA_String_Contents,(APTR)config->sysdirs.sysdata);
	set(App->STR_PA_SS_SYSTEXT,	MUIA_String_Contents,(APTR)config->sysdirs.systext);
	set(App->STR_PA_SS_NEWUSER,	MUIA_String_Contents,(APTR)config->sysdirs.newuser);
	
	sprintf(buffer,"%d",mport->UL.UserEntrys);
	set(App->TX_SS_Users,			MUIA_Text_Contents,(APTR)buffer);

	set(App->TX_SS_BASEDIRS,		MUIA_Text_Contents,(APTR)"kommt noch");

	set(App->TX_SS_NUMBEROFPORTS, MUIA_Text_Contents,(APTR)"ja ja, irgendwann steht hier auch mal was.");

	/************************************/
	/*** SYSTEM SERVICE --> OPTIONS   ***/
	/************************************/

	//set(App->STR_RegVoicenumber,MUIA_String_Contents,(APTR)config->RegVoicenumber);
	//set(App->STR_RegModem,MUIA_String_Contents,(APTR)config->RegVoicenumber);
	//set(App->STR_RegEMail,MUIA_String_Contents,(APTR)config->RegEMail);
	sprintf(buffer,"%d",config->SysopAccountNo);
	set(App->STR_SysopAccountNo,MUIA_String_Contents,(APTR)buffer);
	set(App->STR_DefaultAreaCode,MUIA_String_Contents,(APTR)config->DefaultAreaCode);
	set(App->STR_DefaultLocationNo,MUIA_String_Contents,(APTR)config->DefaultLocationNo);
	set(App->STR_DefaultCountryNo,MUIA_String_Contents,(APTR)config->DefaultCountryNo);

	MaintanceScreenModeID=config->ScreenMode;
	GetDisplayInfoData(0,(APTR)nameinfo,sizeof(NameInfo),DTAG_NAME,config->ScreenMode);
	set(App->STR_PA_Maintance_Screenmode,MUIA_String_Contents,(APTR)nameinfo->Name);
	set(App->CY_SS_Options_Colors,MUIA_Cycle_Active,config->Colors-1);
	set(App->CH_Main,MUIA_Selected,config->CH_Main);

	set(App->CH_SS_loadgui,MUIA_Selected,config->LoadGUI);

	/*** set Font ***/

	set(App->STR_PA_OptionsFont,MUIA_String_Contents,(APTR)config->FontName);

	/******************************************/
	/*** SYSTEM SERVICE --> NETWORK --> TCP ***/
	/******************************************/

	sprintf(buffer,"%d",config->INetConfig.MaxRemoteNodes);
	set(App->STR_SS_TCP_REMOTENODES,		MUIA_String_Contents,(APTR)buffer);

	set(App->STR_SS_TCP_LOCALHOST,		MUIA_String_Contents,(APTR)config->INetConfig.LocalHost);
	set(App->CH_SS_TCP_REMOTEADMIN,		MUIA_Selected,			(APTR)config->INetConfig.RemoteAdmin);

	IPToStr(buffer,config->INetConfig.RemoteIPAddress);
	set(App->STR_SS_TCP_REMOTEIP,			MUIA_String_Contents,(APTR)buffer);

	sprintf(buffer,"%d",config->INetConfig.RemotePort);
	set(App->STR_SS_TCP_Port,				MUIA_String_Contents,(APTR)buffer);

	set(App->CH_SS_TCP_MISC_SUMMERTIME,	MUIA_Selected,			(APTR)config->INetConfig.Summertime);
	

	/*** refresh gui ***/

	MUI_Redraw(App->STR_SystemName,MADF_DRAWOBJECT);
	MUI_Redraw(App->TX_RegistrationID,MADF_DRAWOBJECT);
	MUI_Redraw(App->TX_RegistrationName,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_RegLocation,MADF_DRAWOBJECT);
	//MUI_Redraw(App->STR_RegVoicenumber,MADF_DRAWOBJECT);
	//MUI_Redraw(App->STR_RegModem,MADF_DRAWOBJECT);
	//MUI_Redraw(App->STR_RegEMail,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_MainPortNumber,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_UUCPDomain,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_SysopAccountNo,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_DefaultAreaCode,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_DefaultLocationNo,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_DefaultCountryNo,MADF_DRAWOBJECT);
	MUI_Redraw(App->CY_SS_Options_Colors,MADF_DRAWOBJECT);
	MUI_Redraw(App->STR_PA_OptionsFont,MADF_DRAWOBJECT);
	
	FreeVec(nameinfo);
	return 0;
}
/*
	APTR    STR_RegLocation;
	APTR    STR_MainPortNumber;
	APTR    STR_UUCPDomain;
	APTR    STR_SysopAccountNo;
	APTR    STR_DefaultAreaCode;
	APTR    STR_DefaultLocationNo;
	APTR    STR_DefaultCountryNo;
*/

void GUIToClientConfig(ClientConfig *config)
{
	ULONG ulong;

#ifdef DEBUG
	printf("EDIT-CPORT: %p\n",edit_cport);
#endif
	config->IDNumber=ConvertString(App->STR_CC_IDNUMBER);
//      get(App->CH_CC_loadatstart,MUIA_Selected,&ulong);
//              config->LoadPortatstart=(BYTE)ulong;
	get(App->CH_CC_openmonitor,MUIA_Selected,&ulong);
		config->OpenScreenatstart=(BYTE)ulong;
	get(App->CY_CC_NETWORKTYPE,MUIA_Cycle_Active,&ulong);
		config->Networktype=(BYTE)ulong;
	config->ScreenModeID=ClientScreenModeID;
//      get(App->STR_PA_Screenmode,MUIA_Cycle_Active,&config->ScreenModeID);

	get(App->CH_CC_Workbench,MUIA_Selected,&ulong);
		config->UseWorkbench=(BYTE)ulong;
	get(App->CY_Colors,MUIA_Cycle_Active,&ulong);
		config->Colors=ulong+1;

	GetX(App->STR_PA_ScreenFont,config->ScreenFont,sizeof(config->ScreenFont));
	GetX(App->STR_PA_TerminalFont,config->TermFont,sizeof(config->TermFont));

	GetX(App->STR_PA_Device_Name,config->SerialData.Device_Name,sizeof(config->SerialData.Device_Name));
	config->SerialData.Unit=ConvertString(App->STR_SerConfig_Unitno);
	config->SerialData.IdleRate=ConvertString(App->STR_ClientConfig_Baudrateidle);
	config->SerialData.OnlineBaud=ConvertString(App->STR_Baudrateonline);
	config->SerialData.OfflineBaud=ConvertString(App->STR_CC_Baudrateoffline);

	get(App->CY_CC_Bits,MUIA_Cycle_Active,&ulong);
		config->SerialData.DataBits=(BYTE)ulong+7;

	get(App->CY_CC_Parity,MUIA_Cycle_Active,&ulong);
		config->SerialData.Parity=(BYTE)ulong;

	get(App->CY_CC_Protocoll,MUIA_Cycle_Active,&ulong);
		config->SerialData.Handshake=(BYTE)ulong;

	get(App->CY_CC_StopBits,MUIA_Cycle_Active,&ulong);
		config->SerialData.StopBits=(BYTE)ulong+1;

	get(App->CH_CC_DSR,MUIA_Selected,&ulong);
		config->SerialData.WaitDSR=(BYTE)ulong;

	get(App->CH_CC_DTR,MUIA_Selected,&ulong);
		config->SerialData.DTR_Hangup=(BYTE)ulong;

	config->SerialData.IOBuffersize=ConvertString(App->STR_SerConfig_IOBuffer);
	GetX(App->STR_Modem_CONNECT,config->SerialData.str_CONNECT,sizeof(config->SerialData.str_CONNECT));
	GetX(App->STR_Modem_NOCARRIER,config->SerialData.str_NOCARRIER,sizeof(config->SerialData.str_NOCARRIER));
	GetX(App->STR_Modem_NODIALTONE,config->SerialData.str_NODIALTONE,sizeof(config->SerialData.str_NODIALTONE));
	GetX(App->STR_Modem_RING,config->SerialData.str_RING,sizeof(config->SerialData.str_RING));
	GetX(App->STR_Modem_BUSY,config->SerialData.str_BUSY,sizeof(config->SerialData.str_BUSY));
	GetX(App->STR_Modem_OK,config->SerialData.str_OK,sizeof(config->SerialData.str_OK));
	GetX(App->STR_Modem_TIMEOUT,config->SerialData.str_TIMEOUT,sizeof(config->SerialData.str_TIMEOUT));
	GetX(App->STR_Modem_ERROR,config->SerialData.str_ERROR,sizeof(config->SerialData.str_ERROR));
	GetX(App->STR_Commands_init,config->SerialData.cmd_init,sizeof(config->SerialData.cmd_init));
	GetX(App->STR_Commands_exit,config->SerialData.cmd_exit,sizeof(config->SerialData.cmd_exit));
	GetX(App->STR_Commands_hangup,config->SerialData.cmd_hangup,sizeof(config->SerialData.cmd_hangup));
	GetX(App->STR_Dial_prefix,config->SerialData.dial_dialprefix,sizeof(config->SerialData.dial_dialprefix));
	GetX(App->STR_Dial_suffix,config->SerialData.dial_dialsuffix,sizeof(config->SerialData.dial_dialsuffix));
	get(App->CY_Dial_Dialmode,MUIA_Selected,&ulong);
		config->SerialData.dial_dialmode=(BYTE)ulong;
	GetX(App->STR_Answer_anscmd,config->SerialData.cmd_ANSWER,sizeof(config->SerialData.cmd_ANSWER));
	GetX(App->STR_PA_DeviceParallel,config->ParData.Device_Name,sizeof(config->SerialData.Device_Name));
	get(App->CY_CC_par_cabletype,MUIA_Cycle_Active,&ulong);
		config->ParData.CableType=(BYTE)ulong;

}

BOOL GUIToMainPortConfig(MainPortConfig *config)
{
	ULONG ulong;
	char buffer[100];

	/************************************/
	/*** SYSTEM SERVICE --> MAINTANCE ***/
	/************************************/

	GetX(App->STR_SystemName,config->SystemName,sizeof(config->SystemName));
	GetX(App->TX_RegistrationID,config->RegID,sizeof(config->RegID));
	GetX(App->TX_RegistrationName,config->RegName,sizeof(config->RegName));
	GetX(App->STR_RegLocation,config->RegLocation,sizeof(config->RegLocation));
	GetX(App->STR_MainPortNumber,config->MainPortNumber,sizeof(config->MainPortNumber));
	GetX(App->STR_UUCPDomain,config->UUCPDomain,sizeof(config->UUCPDomain));

	// --------------------------------

	GetX(App->STR_PA_SS_MAINPATH,config->sysdirs.sysdir,sizeof(config->sysdirs.sysdir));
	GetX(App->STR_PA_SS_USERDIR, config->sysdirs.userdir,sizeof(config->sysdirs.userdir));
	GetX(App->STR_PA_SS_SYSDATA, config->sysdirs.sysdata,sizeof(config->sysdirs.sysdata));
	GetX(App->STR_PA_SS_SYSTEXT, config->sysdirs.systext,sizeof(config->sysdirs.systext));
	GetX(App->STR_PA_SS_NEWUSER, config->sysdirs.newuser,sizeof(config->sysdirs.newuser));

	// --------------------------------

	/************************************/
	/*** SYSTEM SERVICE --> OPTIONS ***/
	/************************************/

	config->SysopAccountNo=ConvertString(App->STR_SysopAccountNo);
	GetX(App->STR_DefaultAreaCode,	config->DefaultAreaCode,sizeof(config->DefaultAreaCode));
	GetX(App->STR_DefaultLocationNo,	config->DefaultLocationNo,sizeof(config->DefaultLocationNo));
	GetX(App->STR_DefaultCountryNo,	config->DefaultCountryNo,sizeof(config->DefaultCountryNo));
	if (MaintanceScreenModeID)config->ScreenMode=MaintanceScreenModeID;
	get(App->CY_SS_Options_Colors,MUIA_Cycle_Active,&config->Colors);
	config->Colors=config->Colors+1;
	get(App->CH_Main,MUIA_Selected,&ulong);
	config->CH_Main=(BYTE)ulong;
	get(App->CH_SS_loadgui,MUIA_Selected,&ulong);
	config->LoadGUI=(BYTE)ulong;
	GetX(App->STR_PA_OptionsFont,config->FontName,sizeof(config->FontName));


	/******************************************/
	/*** SYSTEM SERVICE --> NETWORK --> TCP ***/
	/******************************************/

	config->INetConfig.MaxRemoteNodes=ConvertString(App->STR_SS_TCP_REMOTENODES);

	GetX(App->STR_SS_TCP_LOCALHOST,	config->INetConfig.LocalHost,		sizeof(config->INetConfig.LocalHost));

	get(App->CH_SS_TCP_REMOTEADMIN,MUIA_Selected,&ulong);
	config->INetConfig.RemoteAdmin=(char)ulong;

	GetX(App->STR_SS_TCP_REMOTEIP,buffer,20);
	config->INetConfig.RemoteIPAddress=StrToIP(buffer);

	config->INetConfig.RemotePort=(UWORD)ConvertString(App->STR_SS_TCP_Port);

	get(App->CH_SS_TCP_MISC_SUMMERTIME,	MUIA_Selected,&ulong);
	config->INetConfig.Summertime=(char)ulong;

	return 0;
}

ULONG ConvertString(Object *object)
{
	char *buf;
	char *txtbuffer=(char *)AllocVec(100,MEMF_ANY);
	ULONG ulong;
	get(object,MUIA_String_Contents,(APTR)&buf);
	StrToLong(buf,(LONG *)&ulong);
	sprintf(txtbuffer,"%d",ulong);
	set(object,MUIA_String_Contents,(APTR)txtbuffer);
	MUI_Redraw(object,MADF_DRAWOBJECT);
	FreeVec(txtbuffer);
	return ulong;
}


void GetX(Object *object,APTR pointer,ULONG ulong)
{
	char *buf=NULL;
	get(object,MUIA_String_Contents,(APTR)&buf);
	strncpy(pointer,buf,ulong-1);

}


BOOL ClientConfigToGUI(ClientConfig *config)
{
	char buffer[200];
	NameInfo *nameinfo=AllocVec(sizeof(NameInfo),MEMF_ANY);
	ULONG ulong;

#ifdef DEBUG
	printf("EDIT-CPORT: %p\n",edit_cport);
#endif
	sprintf(buffer,"%d",config->IDNumber);
	set(App->STR_CC_IDNUMBER,MUIA_String_Contents,buffer);

//      ulong=(ULONG)config->LoadPortatstart;
//      set(App->CH_CC_loadatstart,MUIA_Selected,ulong);

	ulong=(ULONG)config->OpenScreenatstart;
	set(App->CH_CC_openmonitor,MUIA_Selected,ulong);

	ulong=(ULONG)config->Networktype;
	set(App->CY_CC_NETWORKTYPE,MUIA_Cycle_Active,ulong);
	ClientScreenModeID=config->ScreenModeID;
	GetDisplayInfoData(0,(APTR)nameinfo,sizeof(NameInfo),DTAG_NAME,config->ScreenModeID);

	set(App->STR_PA_Screenmode,MUIA_String_Contents,nameinfo->Name);

	ulong=(ULONG)config->UseWorkbench;
	set(App->CH_CC_Workbench,MUIA_Selected,ulong);

	set(App->CY_Colors,MUIA_Cycle_Active,config->Colors-1);

	sprintf(buffer,"%s",config->ScreenFont);

	set(App->STR_PA_ScreenFont,MUIA_String_Contents,config->ScreenFont);

	sprintf(buffer,"%s",config->TermFont);

	set(App->STR_PA_TerminalFont,MUIA_String_Contents,config->TermFont);

	set(App->STR_PA_Device_Name,MUIA_String_Contents,config->SerialData.Device_Name);

	sprintf(buffer,"%d",config->SerialData.Unit);
	set(App->STR_SerConfig_Unitno,MUIA_String_Contents,buffer);

	sprintf(buffer,"%d",config->SerialData.IdleRate);
	set(App->STR_ClientConfig_Baudrateidle,MUIA_String_Contents,buffer);

	sprintf(buffer,"%d",config->SerialData.OnlineBaud);
	set(App->STR_Baudrateonline,MUIA_String_Contents,buffer);

	sprintf(buffer,"%d",config->SerialData.OfflineBaud);
	set(App->STR_CC_Baudrateoffline,MUIA_String_Contents,buffer);

	set(App->CY_CC_Bits,MUIA_Cycle_Active,config->SerialData.DataBits-7);

	set(App->CY_CC_Parity,MUIA_Cycle_Active,config->SerialData.Parity);

	set(App->CY_CC_Protocoll,MUIA_Cycle_Active,config->SerialData.Handshake);

	set(App->CY_CC_StopBits,MUIA_Cycle_Active,config->SerialData.StopBits-1);

	ulong=(ULONG)config->SerialData.WaitDSR;
	set(App->CH_CC_DSR,MUIA_Selected,ulong);

	ulong=(ULONG)config->SerialData.DTR_Hangup;
	set(App->CH_CC_DTR,MUIA_Selected,ulong);

	sprintf(buffer,"%d",config->SerialData.IOBuffersize);
	set(App->STR_SerConfig_IOBuffer,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_CONNECT);
	set(App->STR_Modem_CONNECT,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_NOCARRIER);
	set(App->STR_Modem_NOCARRIER,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_NODIALTONE);
	set(App->STR_Modem_NODIALTONE,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_RING);
	set(App->STR_Modem_RING,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_BUSY);
	set(App->STR_Modem_BUSY,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_OK);
	set(App->STR_Modem_OK,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_TIMEOUT);
	set(App->STR_Modem_TIMEOUT,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.str_ERROR);
	set(App->STR_Modem_ERROR,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.cmd_init);
	set(App->STR_Commands_init,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.cmd_exit);
	set(App->STR_Commands_exit,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->SerialData.cmd_hangup);
	set(App->STR_Commands_hangup,MUIA_String_Contents,buffer);

//      sprintf(buffer,"hallo",config->SerialData.dial_dialprefix);
	set(App->STR_Dial_prefix,MUIA_String_Contents,config->SerialData.dial_dialprefix);

//      sprintf(buffer,"%s",config->SerialData.dial_dialsuffix);
	set(App->STR_Dial_suffix,MUIA_String_Contents,config->SerialData.dial_dialsuffix);

	ulong=(ULONG)config->SerialData.dial_dialmode;
	set(App->CY_Dial_Dialmode,MUIA_Cycle_Active,ulong);

	sprintf(buffer,"%s",config->SerialData.cmd_ANSWER);
	set(App->STR_Answer_anscmd,MUIA_String_Contents,buffer);

	sprintf(buffer,"%s",config->ParData.Device_Name);
	set(App->STR_PA_DeviceParallel,MUIA_String_Contents,buffer);

	set(App->CY_CC_par_cabletype,MUIA_Cycle_Active,config->ParData.CableType);

	FreeVec(nameinfo);

	return TRUE;
}

BOOL gui_SendGUIMsg( LONG msgid, APTR data)
{
	struct GUIMsg *msg;
	if (guifrontend.GUIOpen && GUIPort)
	{
		msg=AllocVec(sizeof(GUIMsg),MEMF_ANY|MEMF_CLEAR);
		if (!(msg))return FALSE;
		msg->data=data;
		msg->msg.mn_Length=sizeof(GUIMsg);
		msg->msg.mn_ReplyPort=GUIPort;
		msg->cmdid=msgid;
		PutMsg(MainGUIPort,(Message *)msg);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


void UserDataToGUI(UserData *udata)
{
	char buffer[200];
	ULONG ulong;
	sprintf(buffer,"%d",udata->IDNumber);
	set(App->STR_USER_IDNUMBER,MUIA_String_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->IPNumber);
	set(App->TX_USER_IPNUMBER,MUIA_Text_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->Status);
	set(App->TX_USER_STATUS,MUIA_Text_Contents,(APTR)buffer);
	set(App->STR_USER_USERNAME,MUIA_String_Contents,(APTR)udata->Handle);
	set(App->STR_USER_REALNAME,MUIA_String_Contents,(APTR)udata->RealName);
	set(App->STR_USER_STREET,MUIA_String_Contents,(APTR)udata->Street);
	set(App->STR_USER_ZIPCODE,MUIA_String_Contents,(APTR)udata->ZipCode);
	set(App->STR_USER_CITY,MUIA_String_Contents,(APTR)udata->City);
	set(App->STR_USER_COUNTRY,MUIA_String_Contents,(APTR)udata->Country);
	set(App->STR_USER_AREACODE,MUIA_String_Contents,(APTR)udata->AreaCode);
	set(App->STR_USER_PHONENUMBER,MUIA_String_Contents,(APTR)udata->PhoneNo);
	set(App->STR_USER_MODEMNUMBER,MUIA_String_Contents,(APTR)udata->ModemNo);
	set(App->STR_USER_FAXNUMBER,MUIA_String_Contents,(APTR)udata->FaxNo);
	DateToString(buffer,&udata->Birthday,0);
	set(App->STR_USER_BIRTHDAY,MUIA_String_Contents,(APTR)buffer);
	DateToString(buffer,&udata->FirstCall,0);
	set(App->TX_USER_FIRSTCALL,MUIA_Text_Contents,(APTR)buffer);
	DateToString(buffer,&udata->LastCall,0);
	set(App->TX_USER_LASTCALL,MUIA_Text_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->ComputerType);
	set(App->PO_USER_COMPUTER,MUIA_String_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->Access);
	set(App->PO_USER_ACCESS,MUIA_String_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->AccessGroup);
	set(App->PO_USER_ACCESSGROUP,MUIA_String_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->ExpireDays);
	set(App->STR_USER_EXPIREDAYS,MUIA_String_Contents,(APTR)buffer);
	DateToString(buffer,&udata->ExpireDate,0);
	set(App->STR_USER_EXPIREDAY,MUIA_String_Contents,(APTR)buffer);
	set(App->CY_USER_CHARSET,MUIA_Cycle_Active,(APTR)udata->CharSet);
	set(App->CY_USER_MOREMODE,MUIA_Cycle_Active,(APTR)udata->MoreMode);
	set(App->CY_USER_TIMEFORMAT,MUIA_Cycle_Active,(APTR)udata->TimeFormat);
	set(App->CY_USER_DATEFORMAT,MUIA_Cycle_Active,(APTR)udata->DateFormat);
	set(App->CY_USER_LINEFEEDS,MUIA_Cycle_Active,(APTR)udata->TermLineFeeds);
	set(App->CY_USER_TERMTABS,MUIA_Cycle_Active,(APTR)udata->TermTabs);
	set(App->CY_USER_TERMANSI,MUIA_Cycle_Active,(APTR)udata->TermANSI);

	ulong=0;
	if (udata->TermColors==4)ulong=1;
		else if (udata->TermColors==8)ulong=2;
			else	if (udata->TermColors==16)ulong=3;

	set(App->CY_USER_TERMCOLORS,MUIA_Cycle_Active,(APTR)ulong);

	sprintf(buffer,"%d",udata->TermWidth);
	set(App->STR_USER_TERMWIDTH,MUIA_String_Contents,(APTR)buffer);
	sprintf(buffer,"%d",udata->TermLength);
	set(App->STR_USER_TERMLENGTH,MUIA_String_Contents,(APTR)buffer);
}


void GUIToUserData(UserData *udata)
{
	char buffer[200];
	ULONG ulong;
	GetX(App->STR_USER_IDNUMBER,buffer,14);
	StrToLong(buffer,(LONG *)&udata->IDNumber);
	GetX(App->STR_USER_USERNAME,udata->Handle,21);
	GetX(App->STR_USER_REALNAME,udata->RealName,41);
	GetX(App->STR_USER_STREET,udata->Street,41);
	GetX(App->STR_USER_ZIPCODE,udata->ZipCode,11);
	GetX(App->STR_USER_CITY,udata->City,30);
	GetX(App->STR_USER_COUNTRY,udata->Country,4);
	GetX(App->STR_USER_AREACODE,udata->AreaCode,8);
	GetX(App->STR_USER_PHONENUMBER,udata->PhoneNo,25);
	GetX(App->STR_USER_MODEMNUMBER,udata->ModemNo,25);
	GetX(App->STR_USER_FAXNUMBER,udata->FaxNo,25);

/*	DateToString(buffer,&udata->Birthday,0);
	GetX(App->STR_USER_BIRTHDAY,MUIA_String_Contents,(APTR)buffer);
	DateToString(buffer,&udata->FirstCall,0);
	GetX(App->TX_USER_FIRSTCALL,MUIA_Text_Contents,(APTR)buffer);
	DateToString(buffer,&udata->LastCall,0);
	GetX(App->TX_USER_LASTCALL,MUIA_Text_Contents,(APTR)buffer);
*/
	GetX(App->PO_USER_COMPUTER,buffer,10);
	StrToLong(buffer,(LONG *)&udata->ComputerType);

	GetX(App->PO_USER_ACCESS,buffer,10);
	StrToLong(buffer,(LONG *)&udata->Access);

	GetX(App->PO_USER_ACCESSGROUP,buffer,10);
	StrToLong(buffer,(LONG *)&udata->AccessGroup);

	GetX(App->STR_USER_EXPIREDAYS,buffer,6);
	StrToLong(buffer,(LONG *)&ulong);
	udata->ExpireDays=(UWORD)ulong;

/*	GetX(App->STR_USER_EXPIREDAYS,MUIA_String_Contents,(APTR)buffer);
	DateToString(buffer,&udata->ExpireDate,0);
	GetX(App->STR_USER_EXPIREDAY,MUIA_String_Contents,(APTR)buffer);
*/

	get(App->CY_USER_CHARSET,MUIA_Cycle_Active,&ulong);
		udata->CharSet=(UBYTE)ulong;

	get(App->CY_USER_MOREMODE,MUIA_Cycle_Active,&ulong);
		udata->MoreMode=(UBYTE)ulong;

	get(App->CY_USER_TIMEFORMAT,MUIA_Cycle_Active,&ulong);
		udata->TimeFormat=(UBYTE)ulong;

	get(App->CY_USER_DATEFORMAT,MUIA_Cycle_Active,&ulong);
		udata->DateFormat=(UBYTE)ulong;

	get(App->CY_USER_LINEFEEDS,MUIA_Cycle_Active,&ulong);
		udata->TermLineFeeds=(UBYTE)ulong;

	get(App->CY_USER_TERMTABS,MUIA_Cycle_Active,&ulong);
		udata->TermTabs=(UBYTE)ulong;

	get(App->CY_USER_TERMANSI,MUIA_Cycle_Active,&ulong);
		udata->TermANSI=(UBYTE)ulong;

	get(App->CY_USER_TERMCOLORS,MUIA_Cycle_Active,&ulong);
		udata->TermColors=(UBYTE)2<<ulong;

	GetX(App->STR_USER_TERMWIDTH,buffer,4);
	StrToLong(buffer,(LONG *)&ulong);
	udata->TermWidth=(UBYTE)ulong;

	GetX(App->STR_USER_TERMLENGTH,buffer,4);
	StrToLong(buffer,(LONG *)&ulong);
	udata->TermLength=(UBYTE)ulong;

}


void INet_LocalMailServerToGUI(INet_LocalMailServer *server)
{
	char buffer[200];
	ULONG ulong;

	set(App->STR_SS_TCP_MAILBOX_NAME,MUIA_String_Contents,server->Name);
	set(App->STR_SS_TCP_MAILBOX_DOMAIN,MUIA_String_Contents,server->Domain);
	set(App->STR_SS_TCP_MAILBOX_POPHOST,MUIA_String_Contents,server->POP3Host);

	sprintf(buffer,"%d",server->POP3Port);	
	set(App->STR_SS_TCP_MAILBOX_POPPORT,MUIA_String_Contents,buffer);

	set(App->STR_SS_TCP_MAILBOX_SMTPHOST,MUIA_String_Contents,server->SMTPHost);

	sprintf(buffer,"%d",server->SMTPPort);	
	set(App->STR_SS_TCP_MAILBOX_SMTPPORT,MUIA_String_Contents,buffer);

	set(App->PA_SS_TCP_MAILBOX_INVALID_TEXT,MUIA_String_Contents,server->InvalidFilename);
	set(App->STR_SS_TCP_MAILBOX_INVALID_SUBJECT,MUIA_String_Contents,server->InvalidMailSubject);
	set(App->STR_SS_TCP_MAILBOX_INVALID_NAME,MUIA_String_Contents,server->InvalidAddress);
	set(App->PA_SS_TCP_MAILBOX_INBOUND,MUIA_String_Contents,server->InboundPath);
	set(App->PA_SS_TCP_MAILBOX_OUTBOUND,MUIA_String_Contents,server->OutboundPath);

	ulong=(ULONG)server->Flag_Allow8Bit;
	set(App->CH_SS_TCP_MAILBOX_8BIT,MUIA_Selected,ulong);
	ulong=(ULONG)server->Flag_UseAPOP;
	set(App->CH_SS_TCP_MAILBOX_APOP,MUIA_Selected,ulong);
	ulong=(ULONG)server->Flag_ExternalServer;
	set(App->CH_SS_TCP_MAILBOX_EXTSERVER,MUIA_Selected,ulong);
	set(App->TX_SS_TCP_MAILBOX_INVALID_NAME,MUIA_Text_Contents,server->Domain);
}

void INet_LocalUserToGUI(INet_MailUser *user)
{
	char buffer[200];
	ULONG ulong;
	set(App->PO_SS_TCP_MAILBOX_USERS_NAME,				MUIA_String_Contents,	user->Name);
	set(App->CY_SS_TCP_USERS_TYPE,						MUIA_Cycle_Active,		user->UserType);
	set(App->STR_SS_TCP_MAILBOX_USERS_POPID,			MUIA_String_Contents,	user->POPID);
	set(App->CH_SS_TCP_MAILBOX_USERS_POPID_FROMBBS,	MUIA_Selected,				user->Flag_UseBBSUserName);
	set(App->STR_SS_TCP_MAILBOX_USERS_PW,				MUIA_String_Contents,	user->POPPassword);
	set(App->CH_SS_TCP_MAILBOX_USERS_PW_FROMBBS,		MUIA_Selected,				user->Flag_UseBBSPassword);

	sprintf(buffer,"%d",user->HoldMails);
	set(App->STR_SS_TCP_MAILBOX_USERS_HOLDMAILS,		MUIA_String_Contents,	buffer);
	sprintf(buffer,"%d",user->MaxEMails);	
	set(App->STR_SS_TCP_MAILBOX_USERS_MAXMAILS,		MUIA_String_Contents,	buffer);
	sprintf(buffer,"%d",user->Space);
	set(App->STR_SS_TCP_MAILBOX_USERS_SPACE,			MUIA_String_Contents,	buffer);

}

void GUIToINet_LocalMailServer(INet_LocalMailServer *server)
{
	char buffer[200];
	ULONG ulong;

	GetX(App->STR_SS_TCP_MAILBOX_NAME,server->Name,40);
	GetX(App->STR_SS_TCP_MAILBOX_DOMAIN,server->Domain,80);
	GetX(App->STR_SS_TCP_MAILBOX_POPHOST,server->POP3Host,80);

	GetX(App->STR_SS_TCP_MAILBOX_POPPORT,buffer,10);
	StrToLong(buffer,(LONG *)&ulong);
	server->POP3Port=(UWORD)ulong;

	GetX(App->STR_SS_TCP_MAILBOX_SMTPHOST,server->SMTPHost,80);

	GetX(App->STR_SS_TCP_MAILBOX_SMTPPORT,buffer,10);
	StrToLong(buffer,(LONG *)&ulong);
	server->SMTPPort=(UWORD)ulong;

	GetX(App->PA_SS_TCP_MAILBOX_INVALID_TEXT,server->InvalidFilename,80);
	GetX(App->STR_SS_TCP_MAILBOX_INVALID_SUBJECT,server->InvalidMailSubject,80);
	GetX(App->STR_SS_TCP_MAILBOX_INVALID_NAME,server->InvalidAddress,79);
	GetX(App->PA_SS_TCP_MAILBOX_INBOUND,server->InboundPath,80);
	GetX(App->PA_SS_TCP_MAILBOX_OUTBOUND,server->OutboundPath,80);

	get(App->CH_SS_TCP_MAILBOX_8BIT,MUIA_Selected,&ulong);
	server->Flag_Allow8Bit=(char)ulong;

	get(App->CH_SS_TCP_MAILBOX_APOP,MUIA_Selected,&ulong);
	server->Flag_UseAPOP=(char)ulong;

	get(App->CH_SS_TCP_MAILBOX_EXTSERVER,MUIA_Selected,&ulong);
	server->Flag_ExternalServer=(char)ulong;
}

void GUIToINet_LocalUser(INet_MailUser *user)
{
	char buffer[200];
	ULONG ulong;

	GetX(App->PO_SS_TCP_MAILBOX_USERS_NAME,			user->Name,40);
	GetX(App->STR_SS_TCP_MAILBOX_USERS_POPID,			user->POPID,40);
	GetX(App->STR_SS_TCP_MAILBOX_USERS_PW,				user->POPPassword,40);

	get(App->CH_SS_TCP_MAILBOX_USERS_POPID_FROMBBS,MUIA_Selected,&ulong);
	user->Flag_UseBBSUserName=(char)ulong;
	get(App->CH_SS_TCP_MAILBOX_USERS_PW_FROMBBS,MUIA_Selected,&ulong);
	user->Flag_UseBBSPassword=(char)ulong;

	get(App->CY_SS_TCP_USERS_TYPE,MUIA_Cycle_Active,&ulong);
	user->UserType=(char)ulong;

	GetX(App->STR_SS_TCP_MAILBOX_USERS_HOLDMAILS,	buffer,12);
	StrToLong(buffer,(LONG *)&ulong);
	user->HoldMails=(UWORD)ulong;

	GetX(App->STR_SS_TCP_MAILBOX_USERS_MAXMAILS,		buffer,12);
	StrToLong(buffer,(LONG *)&ulong);
	user->MaxEMails=(UWORD)ulong;

	GetX(App->STR_SS_TCP_MAILBOX_USERS_SPACE,			buffer,12);
	StrToLong(buffer,(LONG *)&user->Space);

}


void INet_POP3ServerToGUI(INet_POP3Server *server)
{
	char buffer[200];
	ULONG ulong;

	set(App->STR_SS_TCP_POP_NAME,MUIA_String_Contents,server->Name);
	set(App->STR_SS_TCP_POP_DOMAIN,MUIA_String_Contents,server->Domain);
	set(App->STR_SS_TCP_POP_POPHOST,MUIA_String_Contents,server->POP3Host);
	set(App->STR_SS_TCP_POP_SMTPHOST,MUIA_String_Contents,server->SMTPHost);

	set(App->STR_SS_TCP_POP_USERNAME,MUIA_String_Contents,server->Username);
	set(App->STR_SS_TCP_POP_PASSWORD,MUIA_String_Contents,server->Password);

	set(App->PA_SS_TCP_POP_INVALID_TEXT,MUIA_String_Contents,server->InvalidFilename);
	set(App->STR_SS_TCP_POP_INVALID_SUBJECT,MUIA_String_Contents,server->InvalidMailSubject);
	set(App->STR_SS_TCP_POP_INVALID_NAME,MUIA_String_Contents,server->InvalidAddress);
	
	set(App->PA_SS_TCP_POP_INBOUND,MUIA_String_Contents,server->InboundPath);
	set(App->PA_SS_TCP_POP_OUTBOUND,MUIA_String_Contents,server->OutboundPath);

	ulong=(ULONG)server->Flag_8Bit;
	set(App->CH_SS_TCP_POP_8BIT,MUIA_Selected,ulong);

	ulong=(ULONG)server->Flag_UseAPOP;
	set(App->CH_SS_TCP_POP_APOP,MUIA_Selected,ulong);

	ulong=(ULONG)server->Flag_ExternalServer;
	set(App->CH_SS_TCP_POP_EXTSERVER,MUIA_Selected,ulong);

	ulong=(ULONG)server->Flag_DeleteMails;
	set(App->CH_SS_TCP_POP_DELMAILS,MUIA_Selected,ulong);

	set(App->TX_SS_TCP_POP_INVALID_NAME,MUIA_Text_Contents,server->Domain);
}

void GUIToINet_POP3Server(INet_POP3Server *server)
{
	char buffer[200];
	ULONG ulong;

	GetX(App->STR_SS_TCP_POP_NAME,server->Name,40);
	GetX(App->STR_SS_TCP_POP_DOMAIN,server->Domain,120);
	GetX(App->STR_SS_TCP_POP_POPHOST,server->POP3Host,80);
	GetX(App->STR_SS_TCP_POP_SMTPHOST,server->SMTPHost,80);

	GetX(App->STR_SS_TCP_POP_USERNAME,server->Username,80);
	GetX(App->STR_SS_TCP_POP_PASSWORD,server->Password,80);

	GetX(App->PA_SS_TCP_POP_INVALID_TEXT,server->InvalidFilename,80);
	GetX(App->STR_SS_TCP_POP_INVALID_SUBJECT,server->InvalidMailSubject,80);
	GetX(App->STR_SS_TCP_POP_INVALID_NAME,server->InvalidAddress,80);
	GetX(App->PA_SS_TCP_POP_INBOUND,server->InboundPath,80);
	GetX(App->PA_SS_TCP_POP_OUTBOUND,server->OutboundPath,80);

	get(App->CH_SS_TCP_POP_8BIT,MUIA_Selected,&ulong);
	server->Flag_8Bit=(char)ulong;

	get(App->CH_SS_TCP_POP_APOP,MUIA_Selected,&ulong);
	server->Flag_UseAPOP=(char)ulong;

	get(App->CH_SS_TCP_POP_EXTSERVER,MUIA_Selected,&ulong);
	server->Flag_ExternalServer=(char)ulong;

	get(App->CH_SS_TCP_POP_DELMAILS,MUIA_Selected,&ulong);
	server->Flag_DeleteMails=(char)ulong;
}

void EventToGUI(struct Event *event)
{
	char buffer[200];
	ULONG ulong;

	set(App->STR_REMINDER,MUIA_String_Contents,event->Name);

	DateToString(buffer,&event->BeginDate,DATEFLAG_USEDATE);
	set(App->STR_REMINDER_BEGINDATE,		MUIA_String_Contents,	buffer);

	DateToString(buffer,&event->EndDate,DATEFLAG_USEDATE);
	set(App->STR_REMINDER_ENDDATE,		MUIA_String_Contents,	buffer);

	set(App->CY_REMINDER_DATETYPE,		MUIA_Cycle_Active,	event->DateType);

	sprintf(buffer,"%d",event->IntervalTime);
	set(App->STR_REMINDER_INTERVAL,	MUIA_String_Contents,	buffer);

	switch (event->Multiplicator)
	{
		case MULTIPLICATOR_SECS:
			set(App->CY_REMINDER_INTERVALMULTIPLICATOR,		MUIA_Cycle_Active,	0);
			break;
		case MULTIPLICATOR_MINS:
			set(App->CY_REMINDER_INTERVALMULTIPLICATOR,		MUIA_Cycle_Active,	1);
			break;
		case MULTIPLICATOR_HOURS:
			set(App->CY_REMINDER_INTERVALMULTIPLICATOR,		MUIA_Cycle_Active,	2);
			break;
		case MULTIPLICATOR_DAYS:
			set(App->CY_REMINDER_INTERVALMULTIPLICATOR,		MUIA_Cycle_Active,	3);
			break;
	}
	DateToString(buffer,&event->BeginDate,DATEFLAG_USETIME);
	set(App->STR_REMINDER_BEGINTIME,		MUIA_String_Contents,	buffer);

	DateToString(buffer,&event->EndDate,DATEFLAG_USETIME);
	set(App->STR_REMINDER_ENDTIME,		MUIA_String_Contents,	buffer);


	if (event->UseInterval)
		set(App->CY_REMINDER_INTERVALTYPE,	MUIA_Cycle_Active,0);
	else
		set(App->CY_REMINDER_INTERVALTYPE,	MUIA_Cycle_Active,1);

	set(App->CY_REMINDER_HOLIDAYS,		MUIA_Cycle_Active,	event->Holiday);

	set(App->CH_REMINDER_USEWEEKDAY,	MUIA_Selected, 		event->UseWeekday );

	set(App->CH_REMINDER_MON,			MUIA_Selected, 		event->weekday[1]	);
	set(App->CH_REMINDER_THU,			MUIA_Selected, 		event->weekday[2]	);
	set(App->CH_REMINDER_WEN,			MUIA_Selected, 		event->weekday[3]	);
	set(App->CH_REMINDER_TUR,			MUIA_Selected, 		event->weekday[4]	);
	set(App->CH_REMINDER_FRI,			MUIA_Selected, 		event->weekday[5]	);
	set(App->CH_REMINDER_SAT,			MUIA_Selected, 		event->weekday[6]	);
	set(App->CH_REMINDER_SUN,			MUIA_Selected, 		event->weekday[0]	);

	set(App->CH_REMINDER1,				MUIA_Selected,			event->month[0]	);
	set(App->CH_REMINDER2,				MUIA_Selected, 		event->month[1]	);
	set(App->CH_REMINDER3,				MUIA_Selected, 		event->month[2]	);
	set(App->CH_REMINDER4,				MUIA_Selected, 		event->month[3]	);
	set(App->CH_REMINDER5,				MUIA_Selected, 		event->month[4]	);
	set(App->CH_REMINDER6,				MUIA_Selected, 		event->month[5]	);
	set(App->CH_REMINDER7,				MUIA_Selected, 		event->month[6]	);
	set(App->CH_REMINDER8,				MUIA_Selected, 		event->month[7]	);
	set(App->CH_REMINDER9,				MUIA_Selected, 		event->month[8]	);
	set(App->CH_REMINDER10,				MUIA_Selected, 		event->month[9]	);
	set(App->CH_REMINDER11,				MUIA_Selected, 		event->month[10]	);
	set(App->CH_REMINDER12,				MUIA_Selected, 		event->month[11]	);

	set(App->CY_REMINDER_TYPE,			MUIA_Cycle_Active,	event->EventType	);
	set(App->CH_REMINDER_CLOSEPORT,	MUIA_Selected, 		event->ClosePort	);
}

void GUIToEvent(struct Event *event)
{
	char buffer[200];
	struct Date mydate;
	ULONG ulong;

	GetX(App->STR_REMINDER,event->Name,20);

	GetX(App->STR_REMINDER_BEGINDATE,buffer,30);
	StringToDate(buffer,&event->BeginDate,0);

	GetX(App->STR_REMINDER_ENDDATE,buffer,30);
	StringToDate(buffer,&event->EndDate,0);

	get(App->CY_REMINDER_DATETYPE,		MUIA_Cycle_Active,&ulong);	event->DateType		=(BYTE)ulong;

	GetX(App->STR_REMINDER_INTERVAL,buffer,20);
	StrToLong(buffer,(LONG *)&event->IntervalTime);

	GetX(App->STR_REMINDER_BEGINTIME,buffer,30);
	StringToDate(buffer,&mydate,0);
	event->BeginDate.hours=mydate.hours;
	event->BeginDate.mins=mydate.mins;
	event->BeginDate.secs=mydate.secs;
	CalcDatecode(&event->BeginDate);

	GetX(App->STR_REMINDER_ENDTIME,buffer,30);
	StringToDate(buffer,&mydate,0);
	event->EndDate.hours=mydate.hours;
	event->EndDate.mins=mydate.mins;
	event->EndDate.secs=mydate.secs;
	CalcDatecode(&event->EndDate);

	get(App->CY_REMINDER_INTERVALTYPE, MUIA_Cycle_Active,&ulong);

	if (ulong)
		event->UseInterval=FALSE;
	else
		event->UseInterval=TRUE;

	get(App->CY_REMINDER_INTERVALMULTIPLICATOR, MUIA_Cycle_Active,&ulong);
	
	switch (ulong)
	{
		case 0:
			event->Multiplicator=MULTIPLICATOR_SECS;
			break;
		case 1:
			event->Multiplicator=MULTIPLICATOR_MINS;
			break;
		case 2:
			event->Multiplicator=MULTIPLICATOR_HOURS;
			break;
		case 3:
			event->Multiplicator=MULTIPLICATOR_DAYS;
			break;
	}

	get(App->CY_REMINDER_HOLIDAYS,	MUIA_Cycle_Active,&ulong); event->Holiday			=(UBYTE)ulong;

	get(App->CH_REMINDER_USEWEEKDAY,	MUIA_Selected,&ulong); 		event->UseWeekday		=(UBYTE)ulong;

	get(App->CH_REMINDER_MON,			MUIA_Selected,&ulong);		event->weekday[1]		=(UBYTE)ulong;
	get(App->CH_REMINDER_THU,			MUIA_Selected,&ulong);		event->weekday[2]		=(UBYTE)ulong;
	get(App->CH_REMINDER_WEN,			MUIA_Selected,&ulong);		event->weekday[3]		=(UBYTE)ulong;
	get(App->CH_REMINDER_TUR,			MUIA_Selected,&ulong);		event->weekday[4]		=(UBYTE)ulong;
	get(App->CH_REMINDER_FRI,			MUIA_Selected,&ulong);		event->weekday[5]		=(UBYTE)ulong;
	get(App->CH_REMINDER_SAT,			MUIA_Selected,&ulong);		event->weekday[6]		=(UBYTE)ulong;
	get(App->CH_REMINDER_SUN,			MUIA_Selected,&ulong);		event->weekday[0]		=(UBYTE)ulong;

	get(App->CH_REMINDER1,				MUIA_Selected,&ulong);		event->month[0]		=(UBYTE)ulong;
	get(App->CH_REMINDER2,				MUIA_Selected,&ulong);		event->month[1]		=(UBYTE)ulong;
	get(App->CH_REMINDER3,				MUIA_Selected,&ulong);		event->month[2]		=(UBYTE)ulong;
	get(App->CH_REMINDER4,				MUIA_Selected,&ulong);		event->month[3]		=(UBYTE)ulong;
	get(App->CH_REMINDER5,				MUIA_Selected,&ulong);		event->month[4]		=(UBYTE)ulong;
	get(App->CH_REMINDER6,				MUIA_Selected,&ulong);		event->month[5]		=(UBYTE)ulong;
	get(App->CH_REMINDER7,				MUIA_Selected,&ulong);		event->month[6]		=(UBYTE)ulong;
	get(App->CH_REMINDER8,				MUIA_Selected,&ulong);		event->month[7]		=(UBYTE)ulong;
	get(App->CH_REMINDER9,				MUIA_Selected,&ulong);		event->month[8]		=(UBYTE)ulong;
	get(App->CH_REMINDER10,				MUIA_Selected,&ulong);		event->month[9]		=(UBYTE)ulong;
	get(App->CH_REMINDER11,				MUIA_Selected,&ulong);		event->month[10]		=(UBYTE)ulong;
	get(App->CH_REMINDER12,				MUIA_Selected,&ulong);		event->month[11]		=(UBYTE)ulong;

	get(App->CY_REMINDER_TYPE,			MUIA_Cycle_Active,&ulong);	event->EventType		=(BYTE)ulong;
	get(App->CH_REMINDER_CLOSEPORT,	MUIA_Selected,&ulong);		event->ClosePort		=(BYTE)ulong;

}





