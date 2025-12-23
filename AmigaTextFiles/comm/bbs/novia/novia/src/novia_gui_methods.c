#include "novia_gui_data.h"
#include <exec/memory.h>
#include <novia/novia_gui.h>
#include <libraries/mui.h>

void DoMethods(ObjApp *App)
{

/*****************************************************************************
 ***                                      RETURN-ID's - WINDOW CLIENT LIST ***
 ****************************************************************************/

	DoMethod(App->BT_System_Service,MUIM_Notify,MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SYSTEM_SERVICE	);
	DoMethod(App->BT_Userinfo,		MUIM_Notify,MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_USERINFO		);
	DoMethod(App->BT_Portinfo,		MUIM_Notify,MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_PORTINFO		);
	DoMethod(App->BT_Mailbox,		MUIM_Notify,MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_MAILBOX			);
	DoMethod(App->BT_Config,		MUIM_Notify,MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIG			);

	DoMethod(App->WindowClientList,	MUIM_Notify,MUIA_Window_CloseRequest, TRUE,App->App,2,MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);


/*****************************************************************************
 ***                                         MENUES's - WINDOW CLIENT LIST ***
 ****************************************************************************/


	DoMethod(App->MNlabel1About,				MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_ABOUT			);
	DoMethod(App->MNlabel1AboutMUI,			MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_ABOUTMUI		);
	DoMethod(App->MNlabel1Lockconsule,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_LOCKCONSULE	);
	DoMethod(App->MNlabel1RestartServer,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_RESTARTSERVER);
	DoMethod(App->MNlabel1ResetServer,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_RESETSERVER	);
	DoMethod(App->MNlabel1Quit,				MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_QUIT			);
	DoMethod(App->MNlabel1LocalLogin,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_LOCALLOGIN	);
	DoMethod(App->MNlabel1NoNewUsers,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_NONEWUSERS	);
	DoMethod(App->MNlabel1busyallports,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_BUSYALLPORTS	);
	DoMethod(App->MNlabel1Disable_Base,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLEBASE	);
	DoMethod(App->MNlabel1Config_Base,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGBASE	);
	DoMethod(App->MNlabel1Disable_UDBase,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLEUDBASE);
	DoMethod(App->MNlabel1Config_UDBase,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGUDBASE	);
	DoMethod(App->MNlabel1ChatChannels,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CHATCHANNELS	);
	DoMethod(App->MNlabel1Disable_CC,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLECC	);
	DoMethod(App->MNlabel1Disable_News,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLENEWS	);
	DoMethod(App->MNlabel1Config_News,		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGNEWS	);
	DoMethod(App->MNlabel1Disable_PFiles,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLEPFILES);
	DoMethod(App->MNlabel1Config_PFiles,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGPFILES	);
	DoMethod(App->MNlabel1Disable_GFiles,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLEGFILES);
	DoMethod(App->MNlabel1Config_GFiles,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGGFILES	);
	DoMethod(App->MNlabel1Disable_Votes,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_DISABLEVOTES	);
	DoMethod(App->MNlabel1Config_Config,	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID, GUIID_CONFIGCONFIG	);

	DoMethod(App->WindowClientList,	MUIM_Notify,MUIA_Window_CloseRequest, TRUE,App->App,2,MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);


/*****************************************************************************
 ***                                          RETURN-ID's - SYSTEM SERVICE ***
 ****************************************************************************/



	DoMethod(App->BT_SS_Clients_New,				MUIM_Notify,	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_CLIENTS_NEW);
	DoMethod(App->BT_SS_Clients_Edit,			MUIM_Notify,	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_CLIENTS_EDIT);
	DoMethod(App->BT_SS_Clients_Remove,			MUIM_Notify, 	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_CLIENTS_REMOVE);
	DoMethod(App->BT_SS_Users_New,				MUIM_Notify,	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_USERS_NEW);
	DoMethod(App->BT_SS_Users_Edit,				MUIM_Notify, 	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_USERS_EDIT);
	DoMethod(App->BT_SS_Users_Remove,			MUIM_Notify, 	MUIA_Pressed, FALSE,App->App,2,MUIM_Application_ReturnID, GUIID_SS_USERS_REMOVE);
	DoMethod(App->CY_CC_NETWORKTYPE,				MUIM_Notify, 	MUIA_Cycle_Active,MUIV_EveryTime, App->App, 2, 	MUIM_Application_ReturnID, GUIID_CY_NETWORKCON);
	DoMethod(App->BT_SS_TCP_MAILBOX_HOSTS_NEW,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_NEW);
	DoMethod(App->BT_SS_TCP_MAILBOX_HOSTS_REMOVE,MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_REMOVE);
	DoMethod(App->BT_SS_TCP_MAILBOX_USERS_NEW,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_USER_NEW);
	DoMethod(App->BT_SS_TCP_MAILBOX_USERS_REMOVE,MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_USER_REMOVE);
	DoMethod(App->STR_PO_SS_TCP_MAILBOX_USERS_NAME,	MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_USER_CHANGENAME);

	DoMethod(App->BT_SS_TCP_POP_HOSTS_NEW,			MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_POP3_NEW);
	DoMethod(App->BT_SS_TCP_POP_HOSTS_REMOVE,		MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_SS_TCP_POP3_REMOVE);

//	DoMethod(App->STR_SS_TCP_MAILBOX_NAME,		MUIM_Notify,	MUIA_String_Contents,MUIV_EveryTime,App->App, 2,MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_NAME);

//	DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS,    MUIM_Notify,	MUIA_List_Active, MUIV_List_Active_Off, App->App, 2, MUIM_Application_ReturnID, 88888);
//	DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS,    MUIM_Notify,	MUIV_List_Active_Off, FALSE, App->GR_SS_TCP_MAILBOX_PARAMETERS,2,MUIA_Disabled,TRUE);


	DoMethod(App->BT_SS_ACCEPT,					MUIM_Notify,	MUIA_Pressed, FALSE,App->App, 2,MUIM_Application_ReturnID, GUIID_SS_ACCEPT);
	DoMethod(App->BT_SS_CANCEL,					MUIM_Notify,	MUIA_Pressed, FALSE,App->App, 2,MUIM_Application_ReturnID, GUIID_SS_CANCEL);

	DoMethod(App->STR_PA_Screenmode,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_Screenmode);
	DoMethod(App->STR_PA_ScreenFont,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_ScreenFont);
	DoMethod(App->STR_PA_TerminalFont,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_TerminalFont);
//	DoMethod(App->PA_Device_Name,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_Device_Name);
//	DoMethod(App->STR_SerConfig_Unitno,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SerConfig_UnitnoC);
//	DoMethod(App->STR_CC_Baudrateidle,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_ClientConfig_Baudrateidle);
//	DoMethod(App->STR_CC_Baudrateonline,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Baudrateonline);
//	DoMethod(App->STR_CC_Baudrateoffline,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_ClientConfig_Baudrateonline);
	DoMethod(App->STR_SerConfig_IOBuffer,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SerConfig_IOBuffer);
	DoMethod(App->STR_Modem_CONNECT,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_TriggerValue,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_CONNECT);
	DoMethod(App->STR_Modem_NOCARRIER,			MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_NOCARRIER);
	DoMethod(App->STR_Modem_NODIALTONE,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_NODIALTONE);
	DoMethod(App->STR_Modem_RING,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_RING);
	DoMethod(App->STR_Modem_BUSY,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_BUSY);
	DoMethod(App->STR_Modem_OK,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_OK);
	DoMethod(App->STR_Modem_TIMEOUT,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_TIMEOUT);
	DoMethod(App->STR_Modem_ERROR,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Modem_ERROR);
	DoMethod(App->STR_Commands_init,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Commands_init);
	DoMethod(App->STR_Commands_exit,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Commands_exit);
	DoMethod(App->STR_Commands_hangup,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Commands_hangup);
	DoMethod(App->STR_Dial_prefix,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Dial_prefix);
	DoMethod(App->STR_Dial_suffix,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Dial_suffix);
	DoMethod(App->STR_Answer_anscmd,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Answer_anscmd);
	DoMethod(App->STR_PA_DeviceParallel,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_DeviceParallel);
	DoMethod(App->STR_SystemName,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SystemName);
	DoMethod(App->STR_RegLocation,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_RegLocation);
	DoMethod(App->STR_MainPortNumber,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_MainPortNumber);
	DoMethod(App->STR_UUCPDomain,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_UUCPDomain);
	DoMethod(App->STR_DefaultAreaCode,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_DefaultAreaCode);
	DoMethod(App->STR_DefaultLocationNo,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_DefaultLocationNo);
	DoMethod(App->STR_DefaultCountryNo,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_DefaultCountryNo);
	DoMethod(App->STR_PA_OptionsFont,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_PA_OptionsFont);
	DoMethod(App->STR_SS_Clients_Filter,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SS_Clients_Filter);
	DoMethod(App->STR_SS_Clients_FilterCC,		MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SS_Clients_FilterCC);
	DoMethod(App->STR_RegNumber,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_RegNumber);
	DoMethod(App->STR_Reg_Name,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Reg_Name);
	DoMethod(App->STR_Reg_Location,				MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Reg_Location);
	DoMethod(App->STR_Reg_Voicenumber,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Reg_Voicenumber);
	DoMethod(App->STR_Reg_Modem,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Reg_Modem); 
	DoMethod(App->STR_Reg_EMail,					MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_Reg_EMail);
	DoMethod(App->STR_SysopAccountNo,			MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,App->App,2,MUIM_Application_ReturnID,GUIID_STR_SysopAccountNo);



/*****************************************************************************
 ***                                                        CLOSE REQUESTS ***
 ****************************************************************************/


	DoMethod(App->WindowSystemService,			MUIM_Notify,MUIA_Window_CloseRequest,TRUE,App->App,						2,MUIM_Application_ReturnID, GUIID_SS_CLOSEWIN);
	DoMethod(App->WindowClientConfig,			MUIM_Notify,MUIA_Window_CloseRequest,TRUE,App->WindowClientConfig,	2,MUIM_Application_ReturnID, GUIID_CC_CLOSEWIN);
	DoMethod(App->WindowUserEditor,				MUIM_Notify,MUIA_Window_CloseRequest,TRUE,App->App,						2,MUIM_Application_ReturnID, GUIID_USER_CLOSEWIN);


/*****************************************************************************
 ***                                                                  MISC ***
 ****************************************************************************/


/*	set(App->STR_PA_Screenmode,MUIA_Disabled,TRUE);				
	set(App->STR_PA_Maintance_Screenmode,MUIA_Disabled,TRUE);*/




/*****************************************************************************
 ***                                                         CLIENT CONFIG ***
 ****************************************************************************/

	DoMethod(App->BT_CC_Save,MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_BT_CC_Save); // Action Save
	DoMethod(App->BT_CC_Use,MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_BT_CC_Use); // Action Use
	DoMethod(App->BT_CC_Cancel,MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_BT_CC_Cancel); // Action Cancel


	DoMethod(App->LV_Client_List,	MUIM_Notify,MUIA_Listview_DoubleClick, TRUE,	App->App, 2, MUIM_Application_ReturnID,GUIID_DC_CLIENT_LIST);
	DoMethod(App->LV_SS_Clients,	MUIM_Notify,MUIA_Listview_DoubleClick, TRUE,	App->App, 2, MUIM_Application_ReturnID,GUIID_DC_SS_CLIENT_LIST);
	DoMethod(App->LV_SS_Users,		MUIM_Notify,MUIA_Listview_DoubleClick, TRUE,	App->App, 2, MUIM_Application_ReturnID,GUIID_DC_SS_USERS);


/*****************************************************************************
 ***                                                        CONSULE LOCKED ***
 ****************************************************************************/

	DoMethod(App->BT_CL_OK,		MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_CL_OK		);
	DoMethod(App->BT_CL_CANCEL,	MUIM_Notify,MUIA_Pressed,FALSE,App->App,2,MUIM_Application_ReturnID,GUIID_CL_CANCEL	);


/*****************************************************************************
 ***                                                        REMINDER       ***
 ****************************************************************************/

	DoMethod(App->BT_REMINDER_NEW,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_NEW);
	DoMethod(App->BT_REMINDER_ADD,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_ADD);
	DoMethod(App->BT_REMINDER_REMOVE,MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_REMOVE);
	DoMethod(App->BT_REMINDER_UPDATE,MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_UPDATE);
	DoMethod(App->BT_REMINDER_COPY,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_COPY);
	DoMethod(App->BT_REMINDER_TEST,	MUIM_Notify, MUIA_Pressed, FALSE, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_TEST);

	DoMethod(App->CY_REMINDER_INTERVALTYPE,		 MUIM_Notify, 	MUIA_Cycle_Active,MUIV_EveryTime, App->App, 2, 	MUIM_Application_ReturnID, GUIID_REMINDER_INTERVALTYPE);
	DoMethod(App->CY_REMINDER_HOLIDAYS,				 MUIM_Notify, 	MUIA_Cycle_Active,MUIV_EveryTime, App->App, 2, 	MUIM_Application_ReturnID, GUIID_REMINDER_HOLIDAYS);
	DoMethod(App->CY_REMINDER_DATETYPE,				 MUIM_Notify, 	MUIA_Cycle_Active,MUIV_EveryTime, App->App, 2, 	MUIM_Application_ReturnID, GUIID_REMINDER_DATETYPE);

/*	DoMethod(App->CH_REMINDER_DAYLY,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_DAILY);

	DoMethod(App->CH_REMINDER_MON,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_MON);
	DoMethod(App->CH_REMINDER_THU,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_THU);
	DoMethod(App->CH_REMINDER_WEN,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_WEN);
	DoMethod(App->CH_REMINDER_TUR,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_TUR);
	DoMethod(App->CH_REMINDER_FRI,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_FRI);
	DoMethod(App->CH_REMINDER_SAT,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_SAT);
	DoMethod(App->CH_REMINDER_SUN,	MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_SUN);
	DoMethod(App->CH_REMINDER_WEEKLY,MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_WEEKLY);
	DoMethod(App->CH_REMINDER1C,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_1WEEK);
	DoMethod(App->CH_REMINDER2C,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_2WEEK);
	DoMethod(App->CH_REMINDER3C,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_3WEEK);
	DoMethod(App->CH_REMINDER4C,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_4WEEK);
	DoMethod(App->CH_REMINDER5C,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_5WEEK);
	DoMethod(App->CH_REMINDER_MONTHLY,MUIM_Notify,MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_MONTHLY);
	DoMethod(App->CH_REMINDER1,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_1MONTH);
	DoMethod(App->CH_REMINDER2,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_2MONTH);
	DoMethod(App->CH_REMINDER3,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_3MONTH);
	DoMethod(App->CH_REMINDER4,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_4MONTH);
	DoMethod(App->CH_REMINDER5,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_5MONTH);
	DoMethod(App->CH_REMINDER6,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_6MONTH);
	DoMethod(App->CH_REMINDER7,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_7MONTH);
	DoMethod(App->CH_REMINDER8,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_8MONTH);
	DoMethod(App->CH_REMINDER9,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_9MONTH);
	DoMethod(App->CH_REMINDER10,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_10MONTH);
	DoMethod(App->CH_REMINDER11,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_11MONTH);
	DoMethod(App->CH_REMINDER12,		MUIM_Notify, MUIA_Selected, MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_12MONTH);

	DoMethod(App->CH_REMINDER_USEDATE,MUIM_Notify, MUIA_Selected,MUIV_EveryTime, App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_USEDATE);
	DoMethod(App->CH_REMINDER_CLOSEPORT,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,App->App,	2,	MUIM_Application_ReturnID,GUIID_REMINDER_CLOSEPORT);
*/
//	DoMethod(App->STR_REMINDER,		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime, App->App, 2, MUIM_Application_ReturnID, GUIID_REMINDER_NAME);


/*	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_DAILY,	3,	MUIM_Set, MUIA_Disabled, TRUE );
	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_WEEKLY,	3,	MUIM_Set, MUIA_Disabled, TRUE );
	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_MONTHLY,	3,	MUIM_Set, MUIA_Disabled, TRUE );

	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_DAILY,	3,	MUIM_Set, MUIA_Disabled, FALSE);
	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_WEEKLY,	3,	MUIM_Set, MUIA_Disabled, FALSE);
	DoMethod(App->CH_REMINDER_USEDATE,	MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_MONTHLY,	3,	MUIM_Set, MUIA_Disabled, FALSE);

	DoMethod(App->CH_REMINDER_DAYLY,		MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_DAILY,	3,	MUIM_Set, MUIA_Disabled, TRUE );
	DoMethod(App->CH_REMINDER_DAYLY,		MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_DAILY,	3,	MUIM_Set, MUIA_Disabled, FALSE );

	DoMethod(App->CH_REMINDER_WEEKLY,	MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_WEEKLY,	3,	MUIM_Set, MUIA_Disabled, TRUE );
	DoMethod(App->CH_REMINDER_WEEKLY,	MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_WEEKLY,	3,	MUIM_Set, MUIA_Disabled, FALSE );

	DoMethod(App->CH_REMINDER_MONTHLY,	MUIM_Notify, MUIA_Selected, TRUE,  App->GR_REMINDER_MONTHLY,	3,	MUIM_Set, MUIA_Disabled, TRUE );
	DoMethod(App->CH_REMINDER_MONTHLY,	MUIM_Notify, MUIA_Selected, FALSE, App->GR_REMINDER_MONTHLY,	3,	MUIM_Set, MUIA_Disabled, FALSE );*/

}

