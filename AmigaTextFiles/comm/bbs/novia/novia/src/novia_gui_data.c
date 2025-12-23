#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifdef _DCC	
	#define __inline
#endif

#include "novia_gui_data.h"
#include "novia_gui_dataExtern.h"


struct ObjApp * CreateApp(void)
{
	struct ObjApp * Object;

	APTR	MNlabel1BarLabel4, MNlabel1Config_CC, GR_SS;
	APTR	GR_SS_Options;
	APTR	GR_SS_OPTIONS2, GR_grp_27;
	APTR	GR_SS_Services;
	APTR	GR_SS_SYSTEMMANAGER;
	APTR	GR_SS_BASE, GR_grp_202, GR_SS_BASEVIEW;
	APTR	GR_SS_NEWS, GR_SS_NEWS_LIST, GR_NEWS_ACTION;
	APTR	GR_NEWS_EDIT;
	APTR	GR_SS_PFILES, GR_SS_PFILES_LIST, GR_PFILES_ACTION, GR_PFILES_EDIT;
	APTR	GR_SS_CHAT, GR_SS_NRC, GR_SS_NRC_LIST, GR_grp_151, GR_grp_150, GR_SS_NRC_SETUP;
	APTR	GR_SS_NRC_MISC, GR_SS_NRC_CONFIG;
	APTR	GR_grp_140, GR_grp_145, MEMBERSONLY;
	APTR	GR_grp_138, USEPW, GR_grp_141, BROADCAST;
	APTR	GR_grp_139, GR_grp_142, SYSMSG, GR_grp_146;
	APTR	GR_grp_147;
	APTR	GR_grp_144, GR_grp_148;
	APTR	GR_SS_NRC_IRC;
	APTR	GR_NRC_OPENIRCATSTART;
	APTR	GR_SS_NRC_MEMBERS, GR_NRC_MEMBERS_ACTION;
	APTR	GR_SS_NRC_GROUPS, GR_NRC_GROUPS_ACTION;
	APTR	GR_SS_Network, GR_SS_TCPIP, GR_SS_TCP_MISC;
	APTR	GR_grp_105, GR_grp_106;
	APTR	GR_grp_179, GR_SS_TCP_MAILBOX;
	APTR	GR_SS_TCP_MAILBOX_HOSTLIST, GR_SS_TCP_MAILBOX_HOSTS_ACTION;
	APTR	GR_SS_TCP_MAILBOX_HOSTS_PARA, MAILBOX_NAME, MAILBOX_DOMAIN;
	APTR	GR_SS_TCP_MAILBOX_POPHOST, MAILBOX_POPPORT;
	APTR	GR_SS_TCP_MAILBOX_SMTPHOST, MAILBOX_SMTPPORT;
	APTR	GR_SS_TCP_MAILBOX_FLAGS;
	APTR	GR_SS_TCP_MAILBOX_INVALID_NAME;
	APTR	GR_SS_TCP_MAILBOX_USERLIST, GR_SS_TCP_MAILBOX_USERS_ACTION;
	APTR	GR_SS_TCP_MAILBOX_USER_PARA, MAILBOX_USERS_NAME;
	APTR	GR_SS_TCP_MAILBOX_USERS_TYPE;
	APTR	GR_SS_TCP_MAILBOX_USERS_POPID, MAILBOX_USERS_POPID_FROMBBS;
	APTR	GR_SS_TCP_MAILBOX_USERS_PW, MAILBOX_USERS_PW_FROMBBS;
	APTR	GR_SS_TCP_MAILBOX_USERS_HOLDMAILS;
	APTR	GR_SS_TCP_MAILBOX_USERS_MAXMAILS;
	APTR	GR_SS_TCP_POP, GR_SS_TCP_POP_HOSTLIST;
	APTR	GR_SS_TCP_POP_HOSTS_ACTION, GR_SS_TCP_POP_PARAMETERS;
	APTR	GR_SS_TCP_POP_FLAGS;
	APTR	GR_SS_TCP_POP_INVALID_NAME;
	APTR	GR_SS_TCP_ROUTING, GR_SS_TCP_USERLIST, GR_SS_TCP_USERS_ACTION;
	APTR	GR_SS_TCP_USERS_IN, GR_SS_TCP_USERS_IN_PARA, GR_SS_TCP_USERS_IN_SEARCH;
	APTR	GR_SS_TCP_USERS_OUT, OUT_MOVETO, OUT_FROM;
	APTR	GR_SS_USERS_OUT_FROM, OUT_FROM_AT, OUT_REPLY_TO;
	APTR	GR_SS_USERS_OUT_REPLYTO, OUT_REPLYTO_AT, GR_SS_TCP_USERS_USERS_ACTION;
	APTR	GR_SS_FIDO, GR_SS_FIDOMISC, GR_SS_FIDOACTION, GR_SS_FIDOOPTIONS;
	APTR	GR_SS_FIDONETWORK;
	APTR	GR_SS_FIDOAREAS, GR_SS_FIDOMAREAS_AREAS;
	APTR	GR_SS_FIDOAREAS_AREAS, GR_FIDOAREAS_FLAGS, GR_FIDOAREAS_AREAS;
	APTR	GR_SS_FIDOEXPORT, GR_SS_FIDOEXPORT_EXPORT, GR_SS_FIDOEXPORT_ACTION;
	APTR	GR_FIDOEXPORT_FLAGS, GR_FIDOEXPORT_OPTIONS;
	APTR	GR_grp_43CC;
	APTR	GR_SS_FIDOMAILROUTING;
	APTR	GR_SS_FIDOMAREAS_AREASC, GR_SS_FIDOROUTING_ACTION, GR_FIDOROUTING_OPTIONS;
	APTR	GR_SS_FIDOROUTING_MAILTYPE;
	APTR	GR_SS_Clients, GR_grp_8, GR_grp_26;
	APTR	GR_grp_6, GR_SS_Users;
	APTR	GR_grp_8CC, GR_grp_26CC;
	APTR	GR_grp_6CC, GR_SS_Arc, GR_grp_85, GR_grp_87, GR_grp_86;
	APTR	GR_SS_Editors, GR_grp_8we5CC;
	APTR	GR_grp_87CC2, GR_grp_88;
	APTR	GR_SS_Logs, GR_grp_85CC, GR_grp_87CC, GR_SS_ACCESS, GR_SS_ACCESS_LIST;
	APTR	GR_SS_ACCESS_ACTION, GR_SS_ACCESS_SETTINGS, GR_SS_ACTION;
	APTR	GR_WindowRegistration, GR_grp_28;
	APTR	GR_grp_29;
	APTR	GR_grp_36, GR_grp_37;
	APTR	GR_grp_103;
	APTR	GR_NC_STARTUPMODE;
	APTR	GR_NC_MISC_FLAGS;
	APTR	GR_NC_USEPASSWORD, GR_NC_SERIAL_DEVICE;
	APTR	GR_NC_SERIAL_TOUNIT;
	APTR	GR_NC_SERIAL_SHARED, GR_NC_SERIAL_PARAMETER;
	APTR	GR_NC_SERIAL_BAUDRATE_IDLE;
	APTR	GR_NC_SERIAL_BAUDRATE_ONLINE;
	APTR	GR_NC_SERIAL_BAUDRATE_OFFLINE;
	APTR	GR_NC_SERIAL_FLAGS;
	APTR	GR_NC_SERIAL_IOBUFFER, GR_NC_SERIAL_MODEM;
	APTR	GR_NC_SERIAL_CMD;
	APTR	GR_NC_SERIAL_DIAL;
	APTR	GR_NC_PAR_FROMUNIT;
	APTR	GR_NC_PAR_TOUNIT;
	APTR	GR_NC_TCPIP2_HOST;
	APTR	GR_NC_MONITOR;
	APTR	GR_NC_USEWB; 

	static const struct Hook ScreenmodeToStrHook = { { NULL,NULL },(VOID *)ScreenmodeToStr,NULL,NULL };
	static const struct Hook DisplayListcportHook = { { NULL,NULL },(VOID *)DisplayListcport,NULL,NULL };
	static const struct Hook DisplayListudataHook = { { NULL,NULL },(VOID *)DisplayListudata,NULL,NULL };
	static const struct Hook DisplayListcportminHook = { { NULL,NULL },(VOID *)DisplayListcportmin,NULL,NULL };
	static const struct Hook DisplayListPOP3ServerHook = { { NULL,NULL },(VOID *)DisplayListPOP3Server,NULL,NULL };
	static const struct Hook DisplayListLocalMailServerHook = { { NULL,NULL },(VOID *)DisplayListLocalMailServer,NULL,NULL };
	static const struct Hook DisplayListMailserverUserHook = { { NULL,NULL },(VOID *)DisplayListMailserverUser,NULL,NULL };
	static const struct Hook DisplayListMailserverUpdateHook = { { NULL,NULL },(VOID *)DisplayListMailserverUpdate,NULL,NULL };
	static const struct Hook DisplayListPOP3UpdateHook = { { NULL,NULL },(VOID *)DisplayListPOP3Update,NULL,NULL };
	static const struct Hook DisplayListBaseHook = { { NULL,NULL },(VOID *)DisplayListBase,NULL,NULL };
	static const struct Hook DisplayListBaseRemoveHook = { { NULL,NULL },(VOID *)DisplayListBaseRemove,NULL,NULL };
	static const struct Hook DisplayListEventHook = { { NULL,NULL },(VOID *)DisplayListEvent,NULL,NULL };
	static const struct Hook DisplayListEventRemoveHook = { { NULL,NULL },(VOID *)DisplayListEventRemove,NULL,NULL };
	static const struct Hook DisplayListEventUpdateHook = { { NULL,NULL },(VOID *)DisplayListEventUpdate,NULL,NULL };

	if (!(Object = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC|MEMF_CLEAR)))
		return(NULL);

	Object->STR_TX_CC_IPNUMBER = NULL;
	Object->STR_TX_RegistrationID = NULL;
	Object->STR_TX_RegistrationName = NULL;
	Object->STR_TX_SS_Users = NULL;
	Object->STR_TX_SS_BASEDIRS = NULL;
	Object->STR_TX_SS_NUMBEROFPORTS = NULL;
	Object->STR_TX_SS_BASEVIEW = NULL;
	Object->STR_TX_NRC_ROOMIP = "Serial IP Number:\027[31m \nRoom Name:";
	Object->STR_TX_SS_TCP_MAILBOX_INVALID_NAME = NULL;
	Object->STR_TX_SS_TCP_POP_INVALID_NAME = NULL;
	Object->STR_TX_SS_USERS_OUT_FROM = NULL;
	Object->STR_TX_label_7 = "Welcome to\nNOViA V0.00.05, "__DATE__" "__TIME__"\n Please enter your Registration Number";
	Object->STR_TX_label_9 = "Consule is locked. \n Please enter the Systempassword\n to unlock";
	Object->STR_TX_USER_IPNUMBER = NULL;
	Object->STR_TX_USER_STATUS = NULL;
	Object->STR_TX_USER_FIRSTCALL = NULL;
	Object->STR_TX_USER_LASTCALL = NULL;

	Object->CL_PORT_ACTIONContent[0] = "Online";
	Object->CL_PORT_ACTIONContent[1] = "Offline";
	Object->CL_PORT_ACTIONContent[2] = "Unload";
	Object->CL_PORT_ACTIONContent[3] = NULL;
	Object->STR_GR_grp_10[0] = "Maintance";
	Object->STR_GR_grp_10[1] = "Network-Config";
	Object->STR_GR_grp_10[2] = "Monitor";
	Object->STR_GR_grp_10[3] = "Misc";
	Object->STR_GR_grp_10[4] = NULL;
	Object->CY_CC_NETWORKTYPEContent[0] = "Serial";
	Object->CY_CC_NETWORKTYPEContent[1] = "Parallel";
	Object->CY_CC_NETWORKTYPEContent[2] = "TCP/IP";
	Object->CY_CC_NETWORKTYPEContent[3] = "Envoy";
	Object->CY_CC_NETWORKTYPEContent[4] = "Novell-Netware";
	Object->CY_CC_NETWORKTYPEContent[5] = "Microsoft MSN";
	Object->CY_CC_NETWORKTYPEContent[6] = NULL;
	Object->RA_CC_STARTUPMODEContent[0] = "Online";
	Object->RA_CC_STARTUPMODEContent[1] = "Offline";
	Object->RA_CC_STARTUPMODEContent[2] = "Unload";
	Object->RA_CC_STARTUPMODEContent[3] = "Storage";
	Object->RA_CC_STARTUPMODEContent[4] = NULL;
	Object->STR_GR_SerialConfig[0] = "Device";
	Object->STR_GR_SerialConfig[1] = "Parameter";
	Object->STR_GR_SerialConfig[2] = "Modem";
	Object->STR_GR_SerialConfig[3] = "Commands";
	Object->STR_GR_SerialConfig[4] = "Dial";
	Object->STR_GR_SerialConfig[5] = "Answer";
	Object->STR_GR_SerialConfig[6] = NULL;
	Object->CY_CC_BitsContent[0] = "Seven";
	Object->CY_CC_BitsContent[1] = "Eight";
	Object->CY_CC_BitsContent[2] = NULL;
	Object->CY_CC_ParityContent[0] = "None";
	Object->CY_CC_ParityContent[1] = "Even";
	Object->CY_CC_ParityContent[2] = "Add";
	Object->CY_CC_ParityContent[3] = "1";
	Object->CY_CC_ParityContent[4] = "2";
	Object->CY_CC_ParityContent[5] = NULL;
	Object->CY_CC_ProtocollContent[0] = "None";
	Object->CY_CC_ProtocollContent[1] = "RTS/CTS";
	Object->CY_CC_ProtocollContent[2] = "XOn/XOff";
	Object->CY_CC_ProtocollContent[3] = NULL;
	Object->CY_CC_StopBitsContent[0] = "one";
	Object->CY_CC_StopBitsContent[1] = "two";
	Object->CY_CC_StopBitsContent[2] = NULL;
	Object->CY_Dial_DialmodeContent[0] = "Tone";
	Object->CY_Dial_DialmodeContent[1] = "Pulse";
	Object->CY_Dial_DialmodeContent[2] = NULL;
	Object->CY_CC_par_cabletypeContent[0] = "LappLink-Cable";
	Object->CY_CC_par_cabletypeContent[1] = "ParNet-Cable";
	Object->CY_CC_par_cabletypeContent[2] = "HiSpeed-Cable";
	Object->CY_CC_par_cabletypeContent[3] = NULL;
	Object->CY_ColorsContent[0] = "2";
	Object->CY_ColorsContent[1] = "4";
	Object->CY_ColorsContent[2] = "8";
	Object->CY_ColorsContent[3] = "16";
	Object->CY_ColorsContent[4] = NULL;
	Object->CY_CC_STARTUPSCRIPTContent[0] = "DOS";
	Object->CY_CC_STARTUPSCRIPTContent[1] = "WORKBENCH";
	Object->CY_CC_STARTUPSCRIPTContent[2] = "AREXX";
	Object->CY_CC_STARTUPSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_CC_STARTUPSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_CC_STARTUPSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_CC_STARTUPSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_CC_STARTUPSCRIPTContent[7] = NULL;
	Object->CY_CC_CONNECTSCRIPTContent[0] = "DOS";
	Object->CY_CC_CONNECTSCRIPTContent[1] = "WORKBENCH";
	Object->CY_CC_CONNECTSCRIPTContent[2] = "AREXX";
	Object->CY_CC_CONNECTSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_CC_CONNECTSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_CC_CONNECTSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_CC_CONNECTSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_CC_CONNECTSCRIPTContent[7] = NULL;
	Object->CY_CC_LOGOFFSCRIPTContent[0] = "DOS";
	Object->CY_CC_LOGOFFSCRIPTContent[1] = "WORKBENCH";
	Object->CY_CC_LOGOFFSCRIPTContent[2] = "AREXX";
	Object->CY_CC_LOGOFFSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_CC_LOGOFFSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_CC_LOGOFFSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_CC_LOGOFFSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_CC_LOGOFFSCRIPTContent[7] = NULL;
	Object->STR_GR_SS[0] = "Maintance";
	Object->STR_GR_SS[1] = "Options";
	Object->STR_GR_SS[2] = "Services";
	Object->STR_GR_SS[3] = "Network";
	Object->STR_GR_SS[4] = "Clients";
	Object->STR_GR_SS[5] = "Users";
	Object->STR_GR_SS[6] = "Archivers";
	Object->STR_GR_SS[7] = "Editors";
	Object->STR_GR_SS[8] = "Logs";
	Object->STR_GR_SS[9] = "Accessgroups";
	Object->STR_GR_SS[10] = NULL;
	Object->CY_SS_Options_ColorsContent[0] = "2";
	Object->CY_SS_Options_ColorsContent[1] = "4";
	Object->CY_SS_Options_ColorsContent[2] = "8";
	Object->CY_SS_Options_ColorsContent[3] = "16";
	Object->CY_SS_Options_ColorsContent[4] = NULL;
	Object->STR_GR_SS_Services[0] = "SystemManager";
	Object->STR_GR_SS_Services[1] = "Reminder";
	Object->STR_GR_SS_Services[2] = "Base";
	Object->STR_GR_SS_Services[3] = "News";
	Object->STR_GR_SS_Services[4] = "ProgramFiles";
	Object->STR_GR_SS_Services[5] = "Chat";
	Object->STR_GR_SS_Services[6] = "NoviaRelayChat";
	Object->STR_GR_SS_Services[7] = NULL;

	Object->CY_REMINDER_TYPEContent[0] = "DOS";
	Object->CY_REMINDER_TYPEContent[1] = "Arexx";
	Object->CY_REMINDER_TYPEContent[2] = "NOVIA";
	Object->CY_REMINDER_TYPEContent[3] = NULL;

	Object->CY_REMINDER_DATETYPEContent[0] = "Date";
	Object->CY_REMINDER_DATETYPEContent[1] = "Space";
	Object->CY_REMINDER_DATETYPEContent[2] = "Monthly";
	Object->CY_REMINDER_DATETYPEContent[3] = "Dialy";
	Object->CY_REMINDER_DATETYPEContent[4] = NULL;

	Object->CY_REMINDER_INTERVALTYPEContent[0] = "Interval";
	Object->CY_REMINDER_INTERVALTYPEContent[1] = "fixed Time";
	Object->CY_REMINDER_INTERVALTYPEContent[2] = NULL;

	Object->CY_REMINDER_INTERVALMULTIPLICATORContent[0] = "secs";
	Object->CY_REMINDER_INTERVALMULTIPLICATORContent[1] = "mins";
	Object->CY_REMINDER_INTERVALMULTIPLICATORContent[2] = "hours";
	Object->CY_REMINDER_INTERVALMULTIPLICATORContent[3] = "days";
	Object->CY_REMINDER_INTERVALMULTIPLICATORContent[4] = NULL;

	Object->CY_REMINDER_HOLIDAYSContent[0] = "off";
	Object->CY_REMINDER_HOLIDAYSContent[1] = "only";
	Object->CY_REMINDER_HOLIDAYSContent[2] = "event locked";
	Object->CY_REMINDER_HOLIDAYSContent[3] = NULL;

	Object->CY_NEWS_EDIT_STATUSContent[0] = "Enable";
	Object->CY_NEWS_EDIT_STATUSContent[1] = "Disable";
	Object->CY_NEWS_EDIT_STATUSContent[2] = NULL;
	Object->CY_PFILES_EDIT_STATUSContent[0] = "Enable";
	Object->CY_PFILES_EDIT_STATUSContent[1] = "Disable";
	Object->CY_PFILES_EDIT_STATUSContent[2] = NULL;
	Object->STR_GR_SS_NRC_SETUP[0] = "Configuration";
	Object->STR_GR_SS_NRC_SETUP[1] = "Members";
	Object->STR_GR_SS_NRC_SETUP[2] = "Groups";
	Object->STR_GR_SS_NRC_SETUP[3] = NULL;
	Object->STR_GR_SS_Network[0] = "TCP/IP";
	Object->STR_GR_SS_Network[1] = "FIDO";
	Object->STR_GR_SS_Network[2] = NULL;
	Object->STR_GR_SS_TCPIP[0] = "Misc";
	Object->STR_GR_SS_TCPIP[1] = "Local POP3 Mailboxes";
	Object->STR_GR_SS_TCPIP[2] = "External POP3 Mailboxes";
	Object->STR_GR_SS_TCPIP[3] = "Routing";
	Object->STR_GR_SS_TCPIP[4] = NULL;
	Object->CY_SS_TCP_USERS_TYPEContent[0] = "Local (BBS User)";
	Object->CY_SS_TCP_USERS_TYPEContent[1] = "External (no BBS)";
	Object->CY_SS_TCP_USERS_TYPEContent[2] = NULL;
	Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPEContent[0] = "Username";
	Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPEContent[1] = "Real Name";
	Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPEContent[2] = NULL;
	Object->CY_SS_TCP_USERS_IN_SEARCHContent[0] = "To:";
	Object->CY_SS_TCP_USERS_IN_SEARCHContent[1] = "From:";
	Object->CY_SS_TCP_USERS_IN_SEARCHContent[2] = "Reply-To:";
	Object->CY_SS_TCP_USERS_IN_SEARCHContent[3] = NULL;
	Object->STR_GR_SS_FIDOOPTIONS[0] = "Network";
	Object->STR_GR_SS_FIDOOPTIONS[1] = "Areas";
	Object->STR_GR_SS_FIDOOPTIONS[2] = "Export";
	Object->STR_GR_SS_FIDOOPTIONS[3] = "Routing";
	Object->STR_GR_SS_FIDOOPTIONS[4] = NULL;
	Object->CY_FIDOEXPORT_ARCContent[0] = ".ARC";
	Object->CY_FIDOEXPORT_ARCContent[1] = ".LHA";
	Object->CY_FIDOEXPORT_ARCContent[2] = ".LZX";
	Object->CY_FIDOEXPORT_ARCContent[3] = ".GZ";
	Object->CY_FIDOEXPORT_ARCContent[4] = ".ZIP";
	Object->CY_FIDOEXPORT_ARCContent[5] = ".ZOO";
	Object->CY_FIDOEXPORT_ARCContent[6] = NULL;
	Object->RA_FIDOEXPORT_MAILTYPEContent[0] = "Online";
	Object->RA_FIDOEXPORT_MAILTYPEContent[1] = "Offline";
	Object->RA_FIDOEXPORT_MAILTYPEContent[2] = "Unload";
	Object->RA_FIDOEXPORT_MAILTYPEContent[3] = "Storage";
	Object->RA_FIDOEXPORT_MAILTYPEContent[4] = NULL;
	Object->RA_CC_STARTUPMODECCContent[0] = "Online";
	Object->RA_CC_STARTUPMODECCContent[1] = "Offline";
	Object->RA_CC_STARTUPMODECCContent[2] = "Unload";
	Object->RA_CC_STARTUPMODECCContent[3] = "Storage";
	Object->RA_CC_STARTUPMODECCContent[4] = NULL;
	Object->RA_FIDOROUTING_MAILTYPECContent[0] = "Normal";
	Object->RA_FIDOROUTING_MAILTYPECContent[1] = "Crash";
	Object->RA_FIDOROUTING_MAILTYPECContent[2] = "Hold";
	Object->RA_FIDOROUTING_MAILTYPECContent[3] = NULL;
	Object->CY_SS_Client_SortContent[0] = "Address";
	Object->CY_SS_Client_SortContent[1] = "Network Type";
	Object->CY_SS_Client_SortContent[2] = "Status";
	Object->CY_SS_Client_SortContent[3] = NULL;
	Object->CY_label_3CCContent[0] = "IDNumber";
	Object->CY_label_3CCContent[1] = "IPNumber";
	Object->CY_label_3CCContent[2] = "AcountLevel";
	Object->CY_label_3CCContent[3] = "Username";
	Object->CY_label_3CCContent[4] = "RealName";
	Object->CY_label_3CCContent[5] = NULL;
	Object->CY_label_10Content[0] = "Internal";
	Object->CY_label_10Content[1] = "NOViA PFile";
	Object->CY_label_10Content[2] = "CNET-C";
	Object->CY_label_10Content[3] = "DOS";
	Object->CY_label_10Content[4] = NULL;
	Object->STR_GR_NC_DEFAILT_CONFIG[0] = "Misc";
	Object->STR_GR_NC_DEFAILT_CONFIG[1] = "Network-Config";
	Object->STR_GR_NC_DEFAILT_CONFIG[2] = "Monitor";
	Object->STR_GR_NC_DEFAILT_CONFIG[3] = "Scripts";
	Object->STR_GR_NC_DEFAILT_CONFIG[4] = NULL;
	Object->CY_NC_NETWORKTYPEContent[0] = "Serial";
	Object->CY_NC_NETWORKTYPEContent[1] = "Parallel";
	Object->CY_NC_NETWORKTYPEContent[2] = "TCP/IP";
	Object->CY_NC_NETWORKTYPEContent[3] = "Envoy";
	Object->CY_NC_NETWORKTYPEContent[4] = "Novell-Netware";
	Object->CY_NC_NETWORKTYPEContent[5] = "Microsoft MSN";
	Object->CY_NC_NETWORKTYPEContent[6] = NULL;
	Object->RA_NC_STARTUPMODEContent[0] = "Online";
	Object->RA_NC_STARTUPMODEContent[1] = "Offline";
	Object->RA_NC_STARTUPMODEContent[2] = "Unload";
	Object->RA_NC_STARTUPMODEContent[3] = "Storage";
	Object->RA_NC_STARTUPMODEContent[4] = NULL;
	Object->STR_GR_NC_SERIAL[0] = "Device";
	Object->STR_GR_NC_SERIAL[1] = "Parameter";
	Object->STR_GR_NC_SERIAL[2] = "Modem";
	Object->STR_GR_NC_SERIAL[3] = "Commands";
	Object->STR_GR_NC_SERIAL[4] = "Dial/Answer";
	Object->STR_GR_NC_SERIAL[5] = NULL;
	Object->CY_NC_SERIAL_BITSContent[0] = "Seven";
	Object->CY_NC_SERIAL_BITSContent[1] = "Eight";
	Object->CY_NC_SERIAL_BITSContent[2] = NULL;
	Object->CY_NC_SERIAL_PARITYContent[0] = "None";
	Object->CY_NC_SERIAL_PARITYContent[1] = "Even";
	Object->CY_NC_SERIAL_PARITYContent[2] = "Add";
	Object->CY_NC_SERIAL_PARITYContent[3] = "1";
	Object->CY_NC_SERIAL_PARITYContent[4] = "2";
	Object->CY_NC_SERIAL_PARITYContent[5] = NULL;
	Object->CY_NC_SERIAL_PROTOCOLLContent[0] = "None";
	Object->CY_NC_SERIAL_PROTOCOLLContent[1] = "RTS/CTS";
	Object->CY_NC_SERIAL_PROTOCOLLContent[2] = "XOn/XOff";
	Object->CY_NC_SERIAL_PROTOCOLLContent[3] = NULL;
	Object->CY_NC_SERIAL_STOPBITSContent[0] = "one";
	Object->CY_NC_SERIAL_STOPBITSContent[1] = "two";
	Object->CY_NC_SERIAL_STOPBITSContent[2] = NULL;
	Object->CY_NC_SERIAL_DIALMODEContent[0] = "Tone";
	Object->CY_NC_SERIAL_DIALMODEContent[1] = "Pulse";
	Object->CY_NC_SERIAL_DIALMODEContent[2] = NULL;
	Object->CY_NC_PAR_CABLETYPEContent[0] = "LappLink-Cable";
	Object->CY_NC_PAR_CABLETYPEContent[1] = "ParNet-Cable";
	Object->CY_NC_PAR_CABLETYPEContent[2] = "HiSpeed-Cable";
	Object->CY_NC_PAR_CABLETYPEContent[3] = NULL;
	Object->CY_NC_TCP_GETHOSTNAMEBYContent[0] = "Local DNS";
	Object->CY_NC_TCP_GETHOSTNAMEBYContent[1] = "Remote DNS";
	Object->CY_NC_TCP_GETHOSTNAMEBYContent[2] = NULL;
	Object->CY_NC_COLORSContent[0] = "2";
	Object->CY_NC_COLORSContent[1] = "4";
	Object->CY_NC_COLORSContent[2] = "8";
	Object->CY_NC_COLORSContent[3] = "16";
	Object->CY_NC_COLORSContent[4] = NULL;
	Object->CY_NC_STARTUPSCRIPTContent[0] = "DOS";
	Object->CY_NC_STARTUPSCRIPTContent[1] = "WORKBENCH";
	Object->CY_NC_STARTUPSCRIPTContent[2] = "AREXX";
	Object->CY_NC_STARTUPSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_NC_STARTUPSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_NC_STARTUPSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_NC_STARTUPSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_NC_STARTUPSCRIPTContent[7] = NULL;
	Object->CY_NC_CONNECTSCRIPTContent[0] = "DOS";
	Object->CY_NC_CONNECTSCRIPTContent[1] = "WORKBENCH";
	Object->CY_NC_CONNECTSCRIPTContent[2] = "AREXX";
	Object->CY_NC_CONNECTSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_NC_CONNECTSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_NC_CONNECTSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_NC_CONNECTSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_NC_CONNECTSCRIPTContent[7] = NULL;
	Object->CY_NC_LOGOFFSCRIPTContent[0] = "DOS";
	Object->CY_NC_LOGOFFSCRIPTContent[1] = "WORKBENCH";
	Object->CY_NC_LOGOFFSCRIPTContent[2] = "AREXX";
	Object->CY_NC_LOGOFFSCRIPTContent[3] = "NOVIA-Executable";
	Object->CY_NC_LOGOFFSCRIPTContent[4] = "CNET-C-Program";
	Object->CY_NC_LOGOFFSCRIPTContent[5] = "NOVIA-AREXX";
	Object->CY_NC_LOGOFFSCRIPTContent[6] = "CNET-AREXX";
	Object->CY_NC_LOGOFFSCRIPTContent[7] = NULL;
	Object->CY_USER_CHARSETContent[0] = "ISO 8859-1 (IBM PC, MS Windows)";
	Object->CY_USER_CHARSETContent[1] = "ISO 8859-2";
	Object->CY_USER_CHARSETContent[2] = "ISO 8859-3";
	Object->CY_USER_CHARSETContent[3] = "ISO 8859-4";
	Object->CY_USER_CHARSETContent[4] = "ISO 8859-5 (Cyrillic)";
	Object->CY_USER_CHARSETContent[5] = "ISO 8859-6 (Arabix)";
	Object->CY_USER_CHARSETContent[6] = "ISO 8859-7 (Greek)";
	Object->CY_USER_CHARSETContent[7] = "ISO 8859-8 (Hebrew)";
	Object->CY_USER_CHARSETContent[8] = NULL;
	Object->CY_USER_MOREMODEContent[0] = "No";
	Object->CY_USER_MOREMODEContent[1] = "Yes";
	Object->CY_USER_MOREMODEContent[2] = NULL;
	Object->CY_USER_TIMEFORMATContent[0] = "24 Hours";
	Object->CY_USER_TIMEFORMATContent[1] = "12 Hours (pm/am)";
	Object->CY_USER_TIMEFORMATContent[2] = NULL;
	Object->CY_USER_DATEFORMATContent[0] = "TT-MM-YYYY";
	Object->CY_USER_DATEFORMATContent[1] = "MM-TT-YYYY";
	Object->CY_USER_DATEFORMATContent[2] = NULL;
	Object->CY_USER_LINEFEEDSContent[0] = "Yes";
	Object->CY_USER_LINEFEEDSContent[1] = "No";
	Object->CY_USER_LINEFEEDSContent[2] = NULL;
	Object->CY_USER_TERMTABSContent[0] = "Yes";
	Object->CY_USER_TERMTABSContent[1] = "No";
	Object->CY_USER_TERMTABSContent[2] = NULL;
	Object->CY_USER_TERMANSIContent[0] = "None";
	Object->CY_USER_TERMANSIContent[1] = "Simple";
	Object->CY_USER_TERMANSIContent[2] = "Full";
	Object->CY_USER_TERMANSIContent[3] = NULL;
	Object->CY_USER_TERMCOLORSContent[0] = "Nope";
	Object->CY_USER_TERMCOLORSContent[1] = "Yes";
	Object->CY_USER_TERMCOLORSContent[2] = NULL;

	Object->LV_Client_List = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_Format, "BAR,D=100 BAR P=\33l,D=100 BAR P=\33l,D=200",
		MUIA_List_DisplayHook, &DisplayListcportHook,
	End;

	Object->LV_Client_List = ListviewObject,
		MUIA_HelpNode, "LV_Client_List",
		MUIA_FrameTitle, "Client List",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, Object->LV_Client_List,
	End;

	Object->STR_CC_IDNUMBER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_IDNUMBER",
		MUIA_String_Accept, "0123456798.",
		MUIA_String_MaxLen, 16,
	End;

	Object->TX_CC_IPNUMBER = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_CC_IPNUMBER,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->STR_CC_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_NAME",
	End;

	Object->CY_CC_NETWORKTYPE = CycleObject,
		MUIA_HelpNode, "CY_CC_NETWORKTYPE",
		MUIA_Cycle_Entries, Object->CY_CC_NETWORKTYPEContent,
	End;

	Object->RA_CC_STARTUPMODE = RadioObject,
		MUIA_Weight, 0,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_HelpNode, "RA_CC_STARTUPMODE",
		MUIA_Radio_Entries, Object->RA_CC_STARTUPMODEContent,
	End;

	Object->CH_CC_HIDE = CheckMark(FALSE);

	Object->CH_CC_INTERNALPORT = CheckMark(FALSE);

	Object->CH_CC_ANSIONLY = CheckMark(FALSE);

	Object->PA_Device_Name = PopaslObject,
		MUIA_HelpNode, "PA_Device_Name",
		MUIA_Weight, 4,
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_Device_Name = String("", 80),
		MUIA_Popstring_Button, Object->PA_Device_Name = PopButton(MUII_PopUp),
	End;

	Object->STR_SerConfig_Unitno = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SerConfig_Unitno",
		MUIA_String_Accept, "0123465789",
	End;

	Object->STR_ClientConfig_Baudrateidle = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_ClientConfig_Baudrateidle",
		MUIA_String_Accept, "0123465789",
	End;

	Object->STR_Baudrateonline = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Baudrateonline",
		MUIA_String_Accept, "0123465789",
	End;

	Object->STR_CC_Baudrateoffline = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_Baudrateoffline",
		MUIA_String_Accept, "0123465789",
	End;

	Object->CY_CC_Bits = CycleObject,
		MUIA_HelpNode, "CY_CC_Bits",
		MUIA_Cycle_Entries, Object->CY_CC_BitsContent,
	End;

	Object->CY_CC_Parity = CycleObject,
		MUIA_HelpNode, "CY_CC_Parity",
		MUIA_Cycle_Entries, Object->CY_CC_ParityContent,
	End;

	Object->CY_CC_Protocoll = CycleObject,
		MUIA_HelpNode, "CY_CC_Protocoll",
		MUIA_Cycle_Entries, Object->CY_CC_ProtocollContent,
	End;

	Object->CY_CC_StopBits = CycleObject,
		MUIA_HelpNode, "CY_CC_StopBits",
		MUIA_Cycle_Entries, Object->CY_CC_StopBitsContent,
	End;

	Object->CH_CC_DSR = ImageObject,
		MUIA_Background, MUII_HSHINEBACK,
		MUIA_InputMode, MUIV_InputMode_Toggle,
		MUIA_Image_Spec, MUII_CheckMark,
		MUIA_Image_FreeVert, TRUE,
		MUIA_Selected, TRUE,
		MUIA_ShowSelState, FALSE,
	End;

	Object->STR_SerConfig_IOBuffer = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SerConfig_IOBuffer",
		MUIA_String_Accept, "0123465789",
	End;

	Object->STR_Modem_CONNECT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_CONNECT",
		MUIA_String_Contents, "CONNECT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_NOCARRIER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_NOCARRIER",
		MUIA_String_Contents, "NO CARRIER",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_NODIALTONE = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_NODIALTONE",
		MUIA_String_Contents, "NO DIALTONE",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_RING = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_RING",
		MUIA_String_Contents, "RING",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_BUSY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_BUSY",
		MUIA_String_Contents, "BUSY",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_OK = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_OK",
		MUIA_String_Contents, "OK",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_TIMEOUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_TIMEOUT",
		MUIA_String_Contents, "TIMEOUT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Modem_ERROR = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Modem_ERROR",
		MUIA_String_Contents, "ERROR",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Commands_init = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Commands_init",
		MUIA_String_Contents, "ATZ\r",
	End;

	Object->STR_Commands_exit = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Commands_exit",
	End;

	Object->STR_Commands_hangup = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Commands_hangup",
	End;

	Object->STR_Dial_prefix = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Dial_prefix",
		MUIA_String_Contents, "ATDT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_Dial_suffix = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Dial_suffix",
		MUIA_String_Contents, "\r",
		MUIA_String_MaxLen, 20,
	End;

	Object->CY_Dial_Dialmode = CycleObject,
		MUIA_HelpNode, "CY_Dial_Dialmode",
		MUIA_Cycle_Entries, Object->CY_Dial_DialmodeContent,
	End;

	Object->STR_Answer_anscmd = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Answer_anscmd",
		MUIA_String_MaxLen, 20,
	End;

	Object->PA_DeviceParallel = PopaslObject,
		MUIA_HelpNode, "PA_DeviceParallel",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_DeviceParallel = String("", 80),
		MUIA_Popstring_Button, Object->PA_DeviceParallel = PopButton(MUII_PopFile),
	End;

	Object->CY_CC_par_cabletype = CycleObject,
		MUIA_HelpNode, "CY_CC_par_cabletype",
		MUIA_Cycle_Entries, Object->CY_CC_par_cabletypeContent,
	End;

	Object->GR_Parallel = GroupObject,
		MUIA_HelpNode, "GR_Parallel",
		MUIA_ShowMe, FALSE,
		MUIA_Group_Columns, 2,
		Child, Label("Device"),
		Child, Object->PA_DeviceParallel,
		Child, Label("Cable-Type"),
		Child, Object->CY_CC_par_cabletype,
	End;

	Object->LV_CC_TCPIP2_HOST = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_CC_TCPIP2_HOST = ListviewObject,
		MUIA_HelpNode, "LV_CC_TCPIP2_HOST",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_CC_TCPIP2_HOST,
	End;

	Object->STR_CC_TCPIP2_HOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_TCPIP2_HOST",
	End;

	Object->PO_CC_TCPIP_HOST = PopobjectObject,
		MUIA_HelpNode, "PO_CC_TCPIP_HOST",
		MUIA_Popstring_String, Object->STR_PO_CC_TCPIP_HOST,
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, GroupObject,
			MUIA_HelpNode, "GR_CC_TCPIP2_HOST",
			Child, Object->LV_CC_TCPIP2_HOST,
			Child, Object->STR_CC_TCPIP2_HOST = String("", 80),
			End,
	End;

	Object->STR_CC_TCPIP_PORT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_TCPIP_PORT",
	End;

	Object->GR_TCPIP = GroupObject,
		MUIA_HelpNode, "GR_TCPIP",
		MUIA_ShowMe, FALSE,
		MUIA_Group_Columns, 2,
		Child, Label("Host"),
		Child, Object->PO_CC_TCPIP_HOST,
		Child, Label("Port"),
		Child, Object->STR_CC_TCPIP_PORT,
	End;

	Object->GR_NetworkConfig = GroupObject,
		MUIA_HelpNode, "GR_NetworkConfig",
		Child, Object->GR_SerialConfig = RegisterObject,
			MUIA_Register_Titles, Object->STR_GR_SerialConfig,
			MUIA_HelpNode, "GR_SerialConfig",
			Child, GroupObject,
				MUIA_HelpNode, "GR_ClientConfig_Device",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("Device"),
				Child, Object->PA_Device_Name,
				Child, Label("Unit"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_grp_24C",
					MUIA_Group_Horiz, TRUE,
					Child, Object->STR_SerConfig_Unitno,
					Child, ImageObject,
						MUIA_Image_Spec, 13,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
					Child, ImageObject,
						MUIA_Image_Spec, 14,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
				End,
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_ClientConfig_Parameter",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("Baudrate Idle"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_ClientConfig_Baudrateidle",
					MUIA_Group_Horiz, TRUE,
					Child, Object->STR_ClientConfig_Baudrateidle,
					Child, ImageObject,
						MUIA_Image_Spec, 13,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
					Child, ImageObject,
						MUIA_Image_Spec, 14,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
				End,
				Child, Label("Baudrate Online"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_ClientConfig_Baudrateonline",
					MUIA_Group_Horiz, TRUE,
					Child, Object->STR_Baudrateonline,
					Child, ImageObject,
						MUIA_Image_Spec, 13,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
					Child, ImageObject,
						MUIA_Image_Spec, 14,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
				End,
				Child, Label("Baudrate Offline"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_Baudrate_online",
					MUIA_Group_Horiz, TRUE,
					Child, Object->STR_CC_Baudrateoffline,
					Child, ImageObject,
						MUIA_Image_Spec, 13,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
					Child, ImageObject,
						MUIA_Image_Spec, 14,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
				End,
				Child, Label("Bits"),
				Child, Object->CY_CC_Bits,
				Child, Label("Parity"),
				Child, Object->CY_CC_Parity,
				Child, Label("Protocol"),
				Child, Object->CY_CC_Protocoll,
				Child, Label("Stop Bits"),
				Child, Object->CY_CC_StopBits,
				Child, Label("8N1 Mode"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_Parameter_flags",
					MUIA_Group_Horiz, TRUE,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Check DSR"),
						Child, Object->CH_CC_DSR,
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("DTR-Logoff"),
						Child, Object->CH_CC_DTR = CheckMark(TRUE),
					End,
				End,
				Child, Label("IOBuffersize"),
				Child, GroupObject,
					MUIA_HelpNode, "GR_ClientConfig_IOBuffer",
					MUIA_Group_Horiz, TRUE,
					Child, Object->STR_SerConfig_IOBuffer,
					Child, ImageObject,
						MUIA_Image_Spec, 13,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
					Child, ImageObject,
						MUIA_Image_Spec, 14,
						MUIA_Weight, 0,
						MUIA_Frame, MUIV_Frame_ImageButton,
						MUIA_Image_FreeVert, TRUE,
						MUIA_Image_FreeHoriz, TRUE,
						MUIA_FixHeight, 10,
						MUIA_FixWidth, 10,
					End,
				End,
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_ClientConfig_Modem",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("CONNECT"),
				Child, Object->STR_Modem_CONNECT,
				Child, Label("NOCARRIER"),
				Child, Object->STR_Modem_NOCARRIER,
				Child, Label("NODIALTONE"),
				Child, Object->STR_Modem_NODIALTONE,
				Child, Label("RING"),
				Child, Object->STR_Modem_RING,
				Child, Label("BUSY"),
				Child, Object->STR_Modem_BUSY,
				Child, Label("OK"),
				Child, Object->STR_Modem_OK,
				Child, Label("TIMEOUT"),
				Child, Object->STR_Modem_TIMEOUT,
				Child, Label("ERROR"),
				Child, Object->STR_Modem_ERROR,
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_Commands",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("Modem initialization command"),
				Child, Object->STR_Commands_init,
				Child, Label("Modem exit command"),
				Child, Object->STR_Commands_exit,
				Child, Label("Modem hang up command"),
				Child, Object->STR_Commands_hangup,
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_ClientConfig_Dial",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("Dial prefix"),
				Child, Object->STR_Dial_prefix,
				Child, Label("Dial suffix"),
				Child, Object->STR_Dial_suffix,
				Child, Label("Dialmode"),
				Child, Object->CY_Dial_Dialmode,
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_Answer",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, Label("Answer command"),
				Child, Object->STR_Answer_anscmd,
			End,
		End,
		Child, Object->GR_Parallel,
		Child, Object->GR_TCPIP,
	End;

	Object->PA_TerminalFont = PopaslObject,
		MUIA_HelpNode, "PA_TerminalFont",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_TerminalFont = String("",80),
		MUIA_Popstring_Button, Object->PA_TerminalFont = PopButton(MUII_PopUp),
	End;

	Object->CH_CC_Workbench = CheckMark(FALSE);

	Object->CH_CC_openmonitor = CheckMark(FALSE);

	Object->CY_Colors = CycleObject,
		MUIA_HelpNode, "CY_Colors",
		MUIA_Cycle_Entries, Object->CY_ColorsContent,
	End;

	Object->PA_ScreenFont = PopaslObject,
		MUIA_HelpNode, "PA_ScreenFont",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_ScreenFont = String("", 80),
		MUIA_Popstring_Button, Object->PA_ScreenFont = PopButton(MUII_PopUp),
	End;

	Object->PA_Screenmode = PopaslObject,
		MUIA_HelpNode, "PA_Screenmode",
		MUIA_Popasl_Type, 2,
		MUIA_Popstring_String, Object->STR_PA_Screenmode = String("", 80),
		MUIA_Popstring_Button, Object->PA_Screenmode = PopButton(MUII_PopUp),
		MUIA_Popasl_StopHook, &ScreenmodeToStrHook,
	End;

	Object->PA_CC_STARTUPSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_CC_STARTUPSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_CC_STARTUPSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_CC_STARTUPSCRIPT = PopButton(MUII_PopUp),
	End;

	Object->CY_CC_STARTUPSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_CC_STARTUPSCRIPT",
		MUIA_Cycle_Entries, Object->CY_CC_STARTUPSCRIPTContent,
	End;

	Object->PA_CC_CONNECTSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_CC_CONNECTSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_CC_CONNECTSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_CC_CONNECTSCRIPT = PopButton(MUII_PopUp),
	End;

	Object->CY_CC_CONNECTSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_CC_CONNECTSCRIPT",
		MUIA_Cycle_Entries, Object->CY_CC_CONNECTSCRIPTContent,
	End;

	Object->PA_CC_LOGOFFSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_CC_LOGOFFSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_CC_LOGOFFSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_CC_LOGOFFSCRIPT = PopButton(MUII_PopUp),
	End;

	Object->CY_CC_LOGOFFSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_CC_LOGOFFSCRIPT",
		MUIA_Cycle_Entries, Object->CY_CC_LOGOFFSCRIPTContent,
	End;

	Object->GR_grp_10 = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_grp_10,
		MUIA_HelpNode, "GR_grp_10",
		MUIA_Background, MUII_FILL,
		MUIA_Frame, MUIV_Frame_Group,
		Child, GroupObject,
			MUIA_HelpNode, "GR_MAINTANCE",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 2,
			Child, Label("IDNumber"),
			Child, Object->STR_CC_IDNUMBER,
			Child, Label("IPNumber"),
			Child, Object->TX_CC_IPNUMBER,
			Child, Label("Name"),
			Child, Object->STR_CC_NAME,
			Child, Label("Network type"),
			Child, Object->CY_CC_NETWORKTYPE,
			Child, Label("Statupmode"),
			Child, GroupObject,
				MUIA_HelpNode, "GR_CC_STATUPMODE",
				MUIA_Group_Horiz, TRUE,
				Child, Object->RA_CC_STARTUPMODE,
				Child, HSpace(0),
			End,
			Child, Label("Hide"),
			Child, GroupObject,
				MUIA_HelpNode, "GR_CC_FLAGS",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Group_Horiz, TRUE,
				Child, Object->CH_CC_HIDE,
				Child, Label("Internal Port"),
				Child, Object->CH_CC_INTERNALPORT,
				Child, Label("ANSI only"),
				Child, Object->CH_CC_ANSIONLY,
				Child, HVSpace,
			End,
		End,
		Child, Object->GR_NetworkConfig,
		Child, GroupObject,
			MUIA_HelpNode, "GR_CC_MONITOR",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 2,
			Child, Label("Screenmode"),
			Child, Object->PA_TerminalFont,
			Child, Label("Use Workbench"),
			Child, GroupObject,
				MUIA_HelpNode, "GR_grp_41",
				MUIA_Group_Horiz, TRUE,
				Child, Object->CH_CC_Workbench,
				Child, HVSpace,
				Child, Label("Open monitor at start"),
				Child, Object->CH_CC_openmonitor,
			End,
			Child, TextObject,
				MUIA_Text_PreParse, "\033r",
				MUIA_Text_Contents, "Colors",
				MUIA_InnerLeft, 0,
				MUIA_InnerRight, 0,
			End,
			Child, Object->CY_Colors,
			Child, Label("Screen Font"),
			Child, Object->PA_ScreenFont,
			Child, Label("Screen mode"),
			Child, Object->PA_Screenmode,
		End,
		Child, GroupObject,
			MUIA_HelpNode, "GR_CC_SCRIPTS",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 3,
			Child, Label("Startup script"),
			Child, Object->PA_CC_STARTUPSCRIPT,
			Child, Object->CY_CC_STARTUPSCRIPT,
			Child, Label("Connect script"),
			Child, Object->PA_CC_CONNECTSCRIPT,
			Child, Object->CY_CC_CONNECTSCRIPT,
			Child, Label("Logoff script"),
			Child, Object->PA_CC_LOGOFFSCRIPT,
			Child, Object->CY_CC_LOGOFFSCRIPT,
		End,
	End;

	Object->BT_CC_Save = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Save",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_CC_Save",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_CC_Use = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Use",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_CC_Use",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_CC_Cancel = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Cancel",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_CC_Cancel",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->GR_WindowClientConfig = GroupObject,
		MUIA_Frame, MUIV_Frame_Group,
		Child, Object->GR_grp_10,
		Child, GroupObject,
			MUIA_HelpNode, "GR_grp_35",
			MUIA_Group_Horiz, TRUE,
			MUIA_Group_SameSize, TRUE,
			Child, Object->BT_CC_Save,
			Child, HVSpace,
			Child, Object->BT_CC_Use,
			Child, HVSpace,
			Child, Object->BT_CC_Cancel,
		End,
	End;

	Object->WindowClientConfig = WindowObject,
		MUIA_Window_Title, "Client Configuration",
		MUIA_Window_ID, MAKE_ID('1', 'W', 'I', 'N'),
		WindowContents, Object->GR_WindowClientConfig,
	End;

	Object->STR_SystemName = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_ControlChar, 's',
		MUIA_HelpNode, "STR_SystemName",
	End;

	Object->TX_RegistrationID = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_RegistrationID,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->TX_RegistrationName = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_RegistrationName,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->STR_RegLocation = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_ControlChar, 'l',
		MUIA_HelpNode, "STR_RegLocation",
	End;

	Object->STR_MainPortNumber = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_ControlChar, 'm',
		MUIA_HelpNode, "STR_MainPortNumber",
		MUIA_String_Accept, "0123456789.",
		MUIA_String_MaxLen, 24,
	End;

	Object->STR_UUCPDomain = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_ControlChar, 'u',
		MUIA_HelpNode, "STR_UUCPDomain",
	End;

	Object->PA_SS_MAINPATH = PopaslObject,
		MUIA_HelpNode, "PA_SS_MAINPATH",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_MAINPATH = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_MAINPATH = PopButton(MUII_PopUp),
	End;

	Object->PA_SS_USERDIR = PopaslObject,
		MUIA_HelpNode, "PA_SS_USERDIR",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_USERDIR = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_USERDIR = PopButton(MUII_PopUp),
	End;

	Object->PA_SS_SYSDATA = PopaslObject,
		MUIA_HelpNode, "PA_SS_SYSDATA",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_SYSDATA = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_SYSDATA = PopButton(MUII_PopUp),
	End;

	Object->PA_SS_SYSTEXT = PopaslObject,
		MUIA_HelpNode, "PA_SS_SYSTEXT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_SYSTEXT = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_SYSTEXT = PopButton(MUII_PopUp),
	End;

	Object->PA_SS_NEWUSER = PopaslObject,
		MUIA_HelpNode, "PA_SS_NEWUSER",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_NEWUSER = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_NEWUSER = PopButton(MUII_PopUp),
	End;

	Object->TX_SS_Users = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_Users,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->TX_SS_BASEDIRS = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_BASEDIRS,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->TX_SS_NUMBEROFPORTS = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_NUMBEROFPORTS,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->STR_SysopAccountNo = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SysopAccountNo",
		MUIA_String_Accept, "0123456789.",
	End;

	Object->STR_DefaultAreaCode = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_DefaultAreaCode",
		MUIA_String_MaxLen, 4,
	End;

	Object->STR_DefaultLocationNo = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_DefaultLocationNo",
		MUIA_String_Accept, "0123456789-",
		MUIA_String_MaxLen, 10,
	End;

	Object->STR_DefaultCountryNo = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_DefaultCountryNo",
		MUIA_String_Accept, "0123456789+-",
		MUIA_String_MaxLen, 10,
	End;

	Object->PA_Maintance_Screenmode = PopaslObject,
		MUIA_HelpNode, "PA_Maintance_Screenmode",
		MUIA_Popasl_Type, 2,
		MUIA_Popstring_String, Object->STR_PA_Maintance_Screenmode = String("", 80),
		MUIA_Popstring_Button, Object->PA_Maintance_Screenmode = PopButton(MUII_PopUp),
		MUIA_Popasl_StopHook, &ScreenmodeToStrHook,
	End;

	Object->CH_Main = CheckMark(FALSE);

	Object->CH_SS_loadgui = CheckMark(FALSE);

	GR_grp_27 = GroupObject,
		MUIA_HelpNode, "GR_grp_27",
		MUIA_Group_Horiz, TRUE,
		Child, Object->PA_Maintance_Screenmode,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Workbench"),
			Child, Object->CH_Main,
		End,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Load GUI at start"),
			Child, Object->CH_SS_loadgui,
		End,
	End;

	Object->CY_SS_Options_Colors = CycleObject,
		MUIA_HelpNode, "CY_SS_Options_Colors",
		MUIA_Cycle_Entries, Object->CY_SS_Options_ColorsContent,
	End;

	Object->PA_OptionsFont = PopaslObject,
		MUIA_HelpNode, "PA_OptionsFont",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_OptionsFont = String("", 80),
		MUIA_Popstring_Button, Object->PA_OptionsFont = PopButton(MUII_PopUp),
	End;

	GR_SS_OPTIONS2 = GroupObject,
		MUIA_HelpNode, "GR_SS_OPTIONS2",
		MUIA_Group_Columns, 2,
		Child, Label("Sysop Account no"),
		Child, Object->STR_SysopAccountNo,
		Child, Label("Default Area Code"),
		Child, Object->STR_DefaultAreaCode,
		Child, Label("Default Location Number"),
		Child, Object->STR_DefaultLocationNo,
		Child, Label("Default Country number"),
		Child, Object->STR_DefaultCountryNo,
		Child, Label("Screenmode ID"),
		Child, GR_grp_27,
		Child, Label("Colors"),
		Child, Object->CY_SS_Options_Colors,
		Child, Label("Screen Depth"),
		Child, Object->PA_OptionsFont,
	End;

	GR_SS_Options = GroupObject,
		MUIA_HelpNode, "GR_SS_Options",
		Child, GR_SS_OPTIONS2,
	End;

	Object->PA_Device_Name = PopaslObject,
		MUIA_HelpNode, "PA_Device_Name",
		MUIA_Weight, 4,
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_Device_Name = String("",80),
		MUIA_Popstring_Button, Object->PA_Device_Name = PopButton(MUII_PopUp),
	End;

	Object->TX_SS_BASEVIEW = TextObject,
		MUIA_Background, MUII_SHINE,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_BASEVIEW,
	End;

	Object->BT_label_65 = SimpleButton("<<");

	GR_grp_202 = GroupObject,
		MUIA_HelpNode, "GR_grp_202",
		Child, Object->TX_SS_BASEVIEW,
		Child, Object->BT_label_65,
	End;

	Object->LV_SS_BASEVIEW = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DestructHook, &DisplayListBaseRemoveHook,
		MUIA_List_DisplayHook, &DisplayListBaseHook,
	End;

	Object->LV_SS_BASEVIEW = ListviewObject,
		MUIA_HelpNode, "LV_SS_BASEVIEW",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_BASEVIEW,
	End;

	Object->BT_SS_BASE_OPEN = SimpleButton("_Open");

	Object->BT_SS_BASE_EDIT = SimpleButton("_Edit Parameters");

	Object->BT_SS_BASE_NEW = SimpleButton("_New");

	Object->BT_SS_BASE_REMOVE = SimpleButton("_Remove");

	GR_SS_BASEVIEW = GroupObject,
		MUIA_HelpNode, "GR_SS_BASEVIEW",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_BASE_OPEN,
		Child, HVSpace,
		Child, Object->BT_SS_BASE_EDIT,
		Child, HVSpace,
		Child, Object->BT_SS_BASE_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_BASE_REMOVE,
	End;

	GR_SS_BASE = GroupObject,
		MUIA_HelpNode, "GR_SS_BASE",
		Child, GR_grp_202,
		Child, Object->LV_SS_BASEVIEW,
		Child, GR_SS_BASEVIEW,
	End;

	Object->LV_SS_NEWS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_NEWS = ListviewObject,
		MUIA_HelpNode, "LV_SS_NEWS",
		MUIA_Listview_List, Object->LV_SS_NEWS,
	End;

	Object->STR_SS_NEWS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_NEWS",
	End;

	Object->BT_NEWS_NEW = SimpleButton("New");

	Object->BT_NEWS_ADD = SimpleButton("Add");

	Object->BT_NEWS_REMOVE = SimpleButton("Remove");

	GR_NEWS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_NEWS_ACTION",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_NEWS_NEW,
		Child, Object->BT_NEWS_ADD,
		Child, Object->BT_NEWS_REMOVE,
	End;

	GR_SS_NEWS_LIST = GroupObject,
		MUIA_HelpNode, "GR_SS_NEWS_LIST",
		MUIA_Weight, 50,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Evailable News",
		Child, Object->LV_SS_NEWS,
		Child, Object->STR_SS_NEWS,
		Child, GR_NEWS_ACTION,
	End;

	Object->STR_NEWS_EDIT_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NEWS_EDIT_NAME",
	End;

	Object->LV_NEWS_EDIT_ACCOUTLEVEL = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_NEWS_EDIT_ACCOUTLEVEL = ListviewObject,
		MUIA_HelpNode, "LV_NEWS_EDIT_ACCOUTLEVEL",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_NEWS_EDIT_ACCOUTLEVEL,
	End;

	Object->PO_NEWS_EDIT_ACCOUTLEVEL = PopobjectObject,
		MUIA_HelpNode, "PO_NEWS_EDIT_ACCOUTLEVEL",
		MUIA_Popstring_String, Object->STR_PO_NEWS_EDIT_ACCOUTLEVEL = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_NEWS_EDIT_ACCOUTLEVEL,
	End;

	Object->CY_NEWS_EDIT_STATUS = CycleObject,
		MUIA_HelpNode, "CY_NEWS_EDIT_STATUS",
		MUIA_Cycle_Entries, Object->CY_NEWS_EDIT_STATUSContent,
	End;

	GR_NEWS_EDIT = GroupObject,
		MUIA_HelpNode, "GR_NEWS_EDIT",
		MUIA_Group_Horiz, TRUE,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Name"),
			Child, Object->STR_NEWS_EDIT_NAME,
		End,
		Child, Label("Account Level"),
		Child, Object->PO_NEWS_EDIT_ACCOUTLEVEL,
		Child, Label("Status"),
		Child, Object->CY_NEWS_EDIT_STATUS,
	End;

	GR_SS_NEWS = GroupObject,
		MUIA_HelpNode, "GR_SS_NEWS",
		Child, GR_SS_NEWS_LIST,
		Child, GR_NEWS_EDIT,
	End;

	Object->LV_SS_PFILES = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_PFILES = ListviewObject,
		MUIA_HelpNode, "LV_SS_PFILES",
		MUIA_Listview_List, Object->LV_SS_PFILES,
	End;

	Object->STR_SS_PFILES = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_PFILES",
	End;

	Object->BT_PFILES_NEW = SimpleButton("New");

	Object->BT_PFILES_ADD = SimpleButton("Add");

	Object->BT_PFILES_REMOVE = SimpleButton("Remove");

	GR_PFILES_ACTION = GroupObject,
		MUIA_HelpNode, "GR_PFILES_ACTION",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_PFILES_NEW,
		Child, Object->BT_PFILES_ADD,
		Child, Object->BT_PFILES_REMOVE,
	End;

	GR_SS_PFILES_LIST = GroupObject,
		MUIA_HelpNode, "GR_SS_PFILES_LIST",
		MUIA_Weight, 50,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		Child, Object->LV_SS_PFILES,
		Child, Object->STR_SS_PFILES,
		Child, GR_PFILES_ACTION,
	End;

	Object->STR_PFILES_EDIT_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_PFILES_EDIT_NAME",
	End;

	Object->LV_PFILES_EDIT_ACCOUTLEVEL = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_PFILES_EDIT_ACCOUTLEVEL = ListviewObject,
		MUIA_HelpNode, "LV_PFILES_EDIT_ACCOUTLEVEL",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_PFILES_EDIT_ACCOUTLEVEL,
	End;

	Object->PO_PFILES_EDIT_ACCOUTLEVEL = PopobjectObject,
		MUIA_HelpNode, "PO_PFILES_EDIT_ACCOUTLEVEL",
		MUIA_Popstring_String, Object->STR_PO_PFILES_EDIT_ACCOUTLEVEL = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_PFILES_EDIT_ACCOUTLEVEL,
	End;

	Object->CY_PFILES_EDIT_STATUS = CycleObject,
		MUIA_HelpNode, "CY_PFILES_EDIT_STATUS",
		MUIA_Cycle_Entries, Object->CY_PFILES_EDIT_STATUSContent,
	End;

	GR_PFILES_EDIT = GroupObject,
		MUIA_HelpNode, "GR_PFILES_EDIT",
		MUIA_Group_Horiz, TRUE,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Name"),
			Child, Object->STR_PFILES_EDIT_NAME,
		End,
		Child, Label("Account level"),
		Child, Object->PO_PFILES_EDIT_ACCOUTLEVEL,
		Child, Label("Status"),
		Child, Object->CY_PFILES_EDIT_STATUS,
	End;

	GR_SS_PFILES = GroupObject,
		MUIA_HelpNode, "GR_SS_PFILES",
		Child, GR_SS_PFILES_LIST,
		Child, GR_PFILES_EDIT,
	End;

	GR_SS_CHAT = GroupObject,
		MUIA_HelpNode, "GR_SS_CHAT",
	End;

	Object->TX_NRC_ROOMIP = TextObject,
		MUIA_HelpNode, "TX_NRC_ROOMIP",
		MUIA_Background, MUII_SHINE,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_NRC_ROOMIP,
	End;

	GR_grp_151 = GroupObject,
		MUIA_HelpNode, "GR_grp_151",
		Child, Object->TX_NRC_ROOMIP,
	End;

	Object->LV_SS_NRC_LIST = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_NRC_LIST = ListviewObject,
		MUIA_HelpNode, "LV_SS_NRC_LIST",
		MUIA_Listview_List, Object->LV_SS_NRC_LIST,
	End;

	GR_SS_NRC_LIST = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_LIST",
		MUIA_Weight, 40,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Rooms",
		Child, GR_grp_151,
		Child, Object->LV_SS_NRC_LIST,
	End;

	Object->STR_NRC_ROOMID = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_ROOMID",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 13,
	End;

	Object->STR_NRC_ROOMNAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_ROOMNAME",
		MUIA_String_MaxLen, 40,
	End;

	Object->STR_NRC_PASSWORD = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_PASSWORD",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NRC_ENCRYPT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_ENCRYPT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NRC_MAXIRCUSERS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_MAXIRCUSERS",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 13,
	End;

	Object->STR_NRC_MAXLOCALUSERS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_MAXLOCALUSERS",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 13,
	End;

	Object->CH_NRC_FLAG_INTERNAL = CheckMark(FALSE);

	GR_grp_145 = GroupObject,
		MUIA_HelpNode, "GR_grp_145",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_INTERNAL,
		Child, HVSpace,
	End;

	Object->CH_NRC_FLAG_MEMBERSONLY = CheckMark(FALSE);

	GR_grp_138 = GroupObject,
		MUIA_HelpNode, "GR_grp_138",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_MEMBERSONLY,
		Child, HSpace(0),
	End;

	GR_grp_140 = GroupObject,
		MUIA_HelpNode, "GR_grp_140",
		MUIA_Group_Horiz, TRUE,
		Child, GR_grp_145,
		Child, Label("Members Only"),
		Child, GR_grp_138,
	End;

	Object->CH_NRC_FLAG_USEPW = CheckMark(FALSE);

	GR_grp_141 = GroupObject,
		MUIA_HelpNode, "GR_grp_141",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_USEPW,
		Child, HVSpace,
	End;

	Object->CH_NRC_FLAG_BROADCAST = CheckMark(FALSE);

	GR_grp_142 = GroupObject,
		MUIA_HelpNode, "GR_grp_142",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_BROADCAST,
		Child, HVSpace,
	End;

	Object->CH_NRC_FLAG_SYSMSG = CheckMark(FALSE);

	GR_grp_146 = GroupObject,
		MUIA_HelpNode, "GR_grp_146",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_SYSMSG,
		Child, HSpace(0),
	End;

	GR_grp_139 = GroupObject,
		MUIA_HelpNode, "GR_grp_139",
		MUIA_Group_Horiz, TRUE,
		Child, GR_grp_142,
		Child, Label("System Messages"),
		Child, GR_grp_146,
	End;

	Object->CH_NRC_FLAG_ANONYMOUS = CheckMark(FALSE);

	GR_grp_147 = GroupObject,
		MUIA_HelpNode, "GR_grp_147",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_ANONYMOUS,
		Child, HVSpace,
	End;

	Object->CH_NRC_FLAG_IRCCHANNEL = CheckMark(FALSE);

	Object->CH_NRC_FLAG_REALNAMES = CheckMark(FALSE);

	GR_grp_148 = GroupObject,
		MUIA_HelpNode, "GR_grp_148",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_REALNAMES,
		Child, HSpace(0),
	End;

	GR_grp_144 = GroupObject,
		MUIA_HelpNode, "GR_grp_144",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_FLAG_IRCCHANNEL,
		Child, HVSpace,
		Child, Label("Show Realnames"),
		Child, GR_grp_148,
	End;

	GR_SS_NRC_CONFIG = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_CONFIG",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Miscellanous",
		MUIA_Group_Columns, 4,
		Child, Label("Room ID"),
		Child, Object->STR_NRC_ROOMID,
		Child, Label("Room Name"),
		Child, Object->STR_NRC_ROOMNAME,
		Child, Label("Entry Password"),
		Child, Object->STR_NRC_PASSWORD,
		Child, Label("Entrypt"),
		Child, Object->STR_NRC_ENCRYPT,
		Child, Label("Maximum IRC Users"),
		Child, Object->STR_NRC_MAXIRCUSERS,
		Child, Label("Maximum Local Users"),
		Child, Object->STR_NRC_MAXLOCALUSERS,
		Child, Label("Internal"),
		Child, GR_grp_140,
		Child, Label("Use Entry Password"),
		Child, GR_grp_141,
		Child, Label("Broadcast Messages"),
		Child, GR_grp_139,
		Child, Label("Anonymous Messages"),
		Child, GR_grp_147,
		Child, Label("Room is an IRC Channel"),
		Child, GR_grp_144,
		Child, HVSpace,
		Child, HVSpace,
	End;

	Object->STR_NRC_IRC_IP = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_IRC_IP",
		MUIA_String_Accept, "0123456789.",
		MUIA_String_MaxLen, 16,
	End;

	Object->STR_NRC_IRC_PORT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_IRC_PORT",
		MUIA_String_Contents, "6667",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 6,
	End;

	Object->CH_NRC_OPENIRCATSTART = CheckMark(FALSE);

	GR_NRC_OPENIRCATSTART = GroupObject,
		MUIA_HelpNode, "GR_NRC_OPENIRCATSTART",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NRC_OPENIRCATSTART,
		Child, HVSpace,
	End;

	Object->STR_NRC_IRC_PASSWORD = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_IRC_PASSWORD",
	End;

	Object->STR_NRC_IRC_CHANNELNAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NRC_IRC_CHANNELNAME",
	End;

	GR_SS_NRC_IRC = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_IRC",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Internet Relay Chat",
		MUIA_Group_Columns, 4,
		Child, Label("IRC IP Address"),
		Child, Object->STR_NRC_IRC_IP,
		Child, Label("TCP-Port"),
		Child, Object->STR_NRC_IRC_PORT,
		Child, Label("Open IRC at start"),
		Child, GR_NRC_OPENIRCATSTART,
		Child, Label("IRC Password"),
		Child, Object->STR_NRC_IRC_PASSWORD,
		Child, Label("IRC Cannel"),
		Child, Object->STR_NRC_IRC_CHANNELNAME,
		Child, HVSpace,
		Child, HVSpace,
	End;

	GR_SS_NRC_MISC = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_MISC",
		Child, GR_SS_NRC_CONFIG,
		Child, GR_SS_NRC_IRC,
	End;

	Object->LV_NRC_MEMBERS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_NRC_MEMBERS = ListviewObject,
		MUIA_HelpNode, "LV_NRC_MEMBERS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_NRC_MEMBERS,
	End;

	Object->BT_NRC_MEMBERS_ADD = SimpleButton("Add");

	Object->BT_NRC_MEMBERS_REMOVE = SimpleButton("Remove");

	GR_NRC_MEMBERS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_NRC_MEMBERS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_NRC_MEMBERS_ADD,
		Child, HVSpace,
		Child, HVSpace,
		Child, Object->BT_NRC_MEMBERS_REMOVE,
	End;

	GR_SS_NRC_MEMBERS = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_MEMBERS",
		Child, Object->LV_NRC_MEMBERS,
		Child, GR_NRC_MEMBERS_ACTION,
	End;

	Object->LV_NRC_GROUPS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_NRC_GROUPS = ListviewObject,
		MUIA_HelpNode, "LV_NRC_GROUPS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_NRC_GROUPS,
	End;

	Object->BT_NRC_GROUPS_ADD = SimpleButton("Add");

	Object->BT_NRC_GROUPS_REMOVE = SimpleButton("Remove");

	GR_NRC_GROUPS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_NRC_GROUPS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_NRC_GROUPS_ADD,
		Child, HVSpace,
		Child, HVSpace,
		Child, Object->BT_NRC_GROUPS_REMOVE,
	End;

	GR_SS_NRC_GROUPS = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC_GROUPS",
		Child, Object->LV_NRC_GROUPS,
		Child, GR_NRC_GROUPS_ACTION,
	End;

	GR_SS_NRC_SETUP = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_SS_NRC_SETUP,
		MUIA_HelpNode, "GR_SS_NRC_SETUP",
		Child, GR_SS_NRC_MISC,
		Child, GR_SS_NRC_MEMBERS,
		Child, GR_SS_NRC_GROUPS,
	End;

	GR_grp_150 = GroupObject,
		MUIA_HelpNode, "GR_grp_150",
		Child, GR_SS_NRC_SETUP,
	End;

	GR_SS_NRC = GroupObject,
		MUIA_HelpNode, "GR_SS_NRC",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_NRC_LIST,
		Child, GR_grp_150,
	End;




	GR_SS_Services = RegisterObject,									// SYSTEM SERVICES
		MUIA_Register_Titles, Object->STR_GR_SS_Services,
		MUIA_HelpNode, "GR_SS_Services",
		Child, GroupObject,
			MUIA_HelpNode, "GR_SS_SYSTEMMANAGER",
			Child, GroupObject,
				MUIA_HelpNode, "GR_REMINDER_LISTCC",
				MUIA_Weight, 50,
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_FrameTitle, "Events",
				Child, Object->LV_REMINDERCC = ListviewObject,
					MUIA_Listview_Input, TRUE,
					MUIA_Listview_List, ListObject,
						MUIA_Frame, MUIV_Frame_InputList,
						MUIA_HelpNode, "LV_REMINDERCC",
					End,
				End,
			End,
			Child, Object->STR_REMINDERCC = StringObject,
				MUIA_Frame, MUIV_Frame_String,
				MUIA_HelpNode, "STR_REMINDERCC",
			End,
			Child, GroupObject,
				MUIA_HelpNode, "GR_REMINDER_ACTIONCC",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Group_Horiz, TRUE,
				Child, Object->BT_REMINDER_NEWCC = SimpleButton("New"),
				Child, Object->BT_REMINDER_ADDCC = SimpleButton("Add"),
				Child, Object->BT_REMINDER_REMOVECC = SimpleButton("Remove"),
			End,
		End,

		Child, GroupObject,												// REMINDER
			MUIA_HelpNode, "GR_SS_REMINDER",
			Child, GroupObject,
				MUIA_HelpNode, "GR_REMINDER_LIST",
				MUIA_Weight, 50,
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_FrameTitle, "Events",
				Child, Object->LV_REMINDER = NListviewObject,
					MUIA_HelpNode, "LV_REMINDER",
					MUIA_NListview_NList, NListObject,
		            MUIA_NList_Format, "BAR W=-1,BAR,BAR,BAR",
						MUIA_NList_TitleSeparator, TRUE,
						MUIA_NList_Title, TRUE,
						MUIA_NList_DestructHook,&DisplayListEventRemoveHook,
						MUIA_NList_DisplayHook, &DisplayListEventHook,
					End,
				End,
				Child, Object->STR_REMINDER = StringObject,
					MUIA_Frame, MUIV_Frame_String,
					MUIA_HelpNode, "STR_REMINDER",
				End,
				Child, GroupObject,
					MUIA_HelpNode, "GR_REMINDER_ACTION",
					MUIA_Background, MUII_BACKGROUND,
					MUIA_Group_Horiz, TRUE,
					Child, Object->BT_REMINDER_NEW = TextObject,
						ButtonFrame,
						MUIA_Weight, 0,
						MUIA_Background, MUII_ButtonBack,
						MUIA_Text_Contents, "New",
						MUIA_Text_PreParse, "\033c",
						MUIA_HelpNode, "BT_REMINDER_NEW",
						MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,
					Child, Object->BT_REMINDER_ADD = TextObject,
						ButtonFrame,
						MUIA_Weight, 0,
						MUIA_Background, MUII_ButtonBack,
						MUIA_Text_Contents, "Add",
						MUIA_Text_PreParse, "\033c",
						MUIA_HelpNode, "BT_REMINDER_ADD",
						MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,
					Child, Object->BT_REMINDER_COPY = TextObject,
						ButtonFrame,
						MUIA_Weight, 0,
						MUIA_Background, MUII_ButtonBack,
						MUIA_Text_Contents, "Copy",
						MUIA_Text_PreParse, "\033c",
						MUIA_HelpNode, "BT_REMINDER_COPY",
						MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,
					Child, Object->BT_REMINDER_REMOVE = TextObject,
						ButtonFrame,
						MUIA_Weight, 0,
						MUIA_Background, MUII_ButtonBack,
						MUIA_Text_Contents, "Remove",
						MUIA_Text_PreParse, "\033c",
						MUIA_HelpNode, "BT_REMINDER_REMOVE",
						MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,
					Child, HVSpace,					
					Child, Object->BT_REMINDER_UPDATE = TextObject,
						ButtonFrame,
						MUIA_Weight, 0,
						MUIA_Background, MUII_ButtonBack,
						MUIA_Text_Contents, "Update",
						MUIA_Text_PreParse, "\033c",
						MUIA_HelpNode, "BT_REMINDER_UPDATE",
						MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,
					Child, HVSpace,
					Child, HVSpace,
				End,
			End,
			Child, GroupObject,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Horiz, TRUE,
				Child, Object->CY_REMINDER_DATETYPE = CycleObject,
					MUIA_Cycle_Entries, Object->CY_REMINDER_DATETYPEContent,
				End,
				Child, GroupObject,
					MUIA_Frame, MUIV_Frame_Group,
/*					Child, Object->GR_REMINDER_DATETYPE_EMPTY = GroupObject,
						MUIA_Frame, MUIV_Frame_Group,
						MUIA_Group_Horiz, TRUE,
						Child, HVSpace,
					End,*/
					Child, Object->GR_grp_95 = VGroup,
//						MUIA_Frame, MUIV_Frame_Group,
						MUIA_Group_Horiz, FALSE,
						Child, HVSpace,
						Child, Object->GR_REMINDER_DATETYPE_DATE = GroupObject,
							MUIA_Frame, MUIV_Frame_Group,
							MUIA_Group_Horiz, TRUE,
							Child, Label2("beginn"),
							Child, Object->STR_REMINDER_BEGINDATE = StringObject,
								MUIA_Frame, MUIV_Frame_String,
								MUIA_String_MaxLen, 16,
							End,
							Child, Label2("end"),
							Child, Object->STR_REMINDER_ENDDATE = StringObject,
								MUIA_Frame, MUIV_Frame_String,
								MUIA_String_MaxLen, 16,
							End,
							Child, HVSpace,
						End,
						Child, Object->GR_REMINDER_DATETYPE_MONTHLY = GroupObject,
							MUIA_Frame, MUIV_Frame_Group,
							MUIA_Group_Horiz, TRUE,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("1st"),
								Child, Object->CH_REMINDER1 = CheckMark(FALSE),
							End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("2nd"),
								Child, Object->CH_REMINDER2 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("3rd"),
								Child, Object->CH_REMINDER3 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("4th"),
								Child, Object->CH_REMINDER4 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("5th"),
								Child, Object->CH_REMINDER5 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("6th"),
								Child, Object->CH_REMINDER6 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("7th"),
								Child, Object->CH_REMINDER7 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("8th"),
								Child, Object->CH_REMINDER8 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("9th"),
								Child, Object->CH_REMINDER9 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("10th"),
								Child, Object->CH_REMINDER10 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("11th"),
								Child, Object->CH_REMINDER11 = CheckMark(FALSE),
								End,
							Child, GroupObject,
								MUIA_Group_Columns, 2,
								Child, Label2("12th"),
								Child, Object->CH_REMINDER12 = CheckMark(FALSE),
								End,
							Child, HVSpace,
						End,
					End,
				End,
			End,

			Child, GroupObject,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_Group_Columns, 2,
				Child, GroupObject,
					MUIA_Group_Horiz, TRUE,
					Child, HVSpace,
					Child, Label("Use Weekday"),
					Child, Object->CH_REMINDER_USEWEEKDAY = CheckMark(FALSE),
				End,
				Child, GroupObject,
					MUIA_Frame, MUIV_Frame_Group,
					MUIA_Group_Horiz, TRUE,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Mon"),
						Child, Object->CH_REMINDER_MON = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Thu"),
						Child, Object->CH_REMINDER_THU = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Wen"),
						Child, Object->CH_REMINDER_WEN = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Tur"),
						Child, Object->CH_REMINDER_TUR = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Fri"),
						Child, Object->CH_REMINDER_FRI = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Sat"),
						Child, Object->CH_REMINDER_SAT = CheckMark(FALSE),
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Sun"),
						Child, Object->CH_REMINDER_SUN = CheckMark(FALSE),
					End,
					Child, HVSpace,
					Child, Label("Feiertag"),
					Child, Object->CY_REMINDER_HOLIDAYS = CycleObject,
						MUIA_Weight, 0,
						MUIA_Cycle_Entries, Object->CY_REMINDER_HOLIDAYSContent,
					End,
				End,
				Child, GroupObject,
					MUIA_Group_Horiz, TRUE,
					Child, Object->CY_REMINDER_INTERVALTYPE = CycleObject,
						MUIA_Weight, 0,
						MUIA_Cycle_Entries, Object->CY_REMINDER_INTERVALTYPEContent,
					End,
				End,
				Child, GroupObject,
					MUIA_Frame, MUIV_Frame_Group,
					MUIA_Group_Horiz, TRUE,
					Child, Label("begin"),
					Child, Object->STR_REMINDER_BEGINTIME = StringObject,
						MUIA_Frame, MUIV_Frame_String,
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("end"),
						Child, Object->STR_REMINDER_ENDTIME = StringObject,
							MUIA_Frame, MUIV_Frame_String,
						End,
					End,
					Child, GroupObject,
						MUIA_Group_Columns, 2,
						Child, Label2("Interval"),
						Child, Object->STR_REMINDER_INTERVAL = StringObject,
							MUIA_Frame, MUIV_Frame_String,
						End,
					End,
					Child, GroupObject,
						MUIA_Group_Horiz, TRUE,
						Child, Object->CY_REMINDER_INTERVALMULTIPLICATOR = CycleObject,
							MUIA_Weight, 0,
							MUIA_Cycle_Entries, Object->CY_REMINDER_INTERVALMULTIPLICATORContent,
						End,
					End,
					Child, HVSpace,
				End,
			End,



			Child, GroupObject,
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_FrameTitle, "Event Type",
				MUIA_Group_Horiz, TRUE,
				Child, Label("Event Type"),
				Child, Object->CY_REMINDER_TYPE = CycleObject,
					MUIA_Weight, 0,
					MUIA_HelpNode, "CY_REMINDER_TYPE",
					MUIA_Cycle_Entries, Object->CY_REMINDER_TYPEContent,
				End,
				Child, GroupObject,
					MUIA_Group_Columns, 2,
					MUIA_Weight, 0,
					Child, Label2("Close Port"),
					Child, Object->CH_REMINDER_CLOSEPORT = CheckMark(FALSE),
				End,
				Child, GroupObject,
					MUIA_Group_Columns, 2,
					Child, Label2("Remove if finish"),
					Child, Object->CH_REMINDER_DEL_END = CheckMark(FALSE),
				End,
				Child, Label2("Program Name"),
				Child, Object->PA_REMINDER_PROGNAME = PopaslObject,
					MUIA_HelpNode, "PO_REMINDER_PROGNAME",
					MUIA_Popasl_Type, 0,
					MUIA_Popstring_String, Object->STR_PA_REMINDER_PROGNAME = String("", 80),
					MUIA_Popstring_Button, Object->PA_REMINDER_PROGNAME = PopButton(MUII_PopUp),
				End,
				Child, Object->BT_REMINDER_TEST = TextObject,
					ButtonFrame,
					MUIA_Weight, 0,
					MUIA_Background, MUII_ButtonBack,
					MUIA_Text_Contents, "Test event",
					MUIA_Text_PreParse, "\033c",
					MUIA_HelpNode, "BT_REMINDER_TEST",
					MUIA_InputMode, MUIV_InputMode_RelVerify,
				End,
			End,
		End,

	
	

		Child, GR_SS_BASE,
		Child, GR_SS_NEWS,
		Child, GR_SS_PFILES,
		Child, GR_SS_CHAT,
		Child, GR_SS_NRC,
	End;

	Object->STR_SS_TCP_REMOTENODES = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_REMOTENODES",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 12,
	End;

	Object->STR_SS_TCP_LOCALHOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_LOCALHOST",
	End;

	Object->CH_SS_TCP_REMOTEADMIN = CheckMark(FALSE);

	GR_grp_105 = GroupObject,
		MUIA_HelpNode, "GR_grp_105",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_SS_TCP_REMOTEADMIN,
		Child, HVSpace,
	End;

	Object->STR_SS_TCP_REMOTEIP = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_REMOTEIP",
	End;

	Object->STR_SS_TCP_Port = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_Port",
	End;

	GR_grp_106 = GroupObject,
		MUIA_HelpNode, "GR_grp_106",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_REMOTEIP,
		Child, Label("Port"),
		Child, Object->STR_SS_TCP_Port,
	End;

	Object->CH_SS_TCP_MISC_SUMMERTIME = CheckMark(FALSE);

	GR_grp_179 = GroupObject,
		MUIA_HelpNode, "GR_grp_179",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_SS_TCP_MISC_SUMMERTIME,
		Child, HVSpace,
	End;

	GR_SS_TCP_MISC = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MISC",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Misc",
		MUIA_Group_Columns, 2,
		Child, Label("Remote Nodes"),
		Child, Object->STR_SS_TCP_REMOTENODES,
		Child, Label("Local Host"),
		Child, Object->STR_SS_TCP_LOCALHOST,
		Child, Label("Allow remote Administration"),
		Child, GR_grp_105,
		Child, Label("Remote IP Address"),
		Child, GR_grp_106,
		Child, Label("Summertime"),
		Child, GR_grp_179,
	End;

	Object->LV_SS_TCP_MAILBOX_HOSTS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListLocalMailServerHook,
	End;

	Object->LV_SS_TCP_MAILBOX_HOSTS = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_MAILBOX_HOSTS",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, Object->LV_SS_TCP_MAILBOX_HOSTS,
	End;

	Object->BT_SS_TCP_MAILBOX_HOSTS_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_MAILBOX_HOSTS_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_TCP_MAILBOX_HOSTS_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_MAILBOX_HOSTS_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_TCP_MAILBOX_HOSTS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_HOSTS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_TCP_MAILBOX_HOSTS_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_TCP_MAILBOX_HOSTS_REMOVE,
	End;

	GR_SS_TCP_MAILBOX_HOSTLIST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_HOSTLIST",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Name",
		Child, Object->LV_SS_TCP_MAILBOX_HOSTS,
		Child, GR_SS_TCP_MAILBOX_HOSTS_ACTION,
	End;

	Object->STR_SS_TCP_MAILBOX_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_NAME",
	End;

	Object->STR_SS_TCP_MAILBOX_DOMAIN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_DOMAIN",
	End;

	Object->STR_SS_TCP_MAILBOX_POPHOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_POPHOST",
	End;

	Object->STR_SS_TCP_MAILBOX_POPPORT = StringObject,
		MUIA_Weight, 20,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_POPPORT",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 7,
	End;

	GR_SS_TCP_MAILBOX_POPHOST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_POPHOST",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_POPHOST,
		Child, Label("POP3 Port"),
		Child, Object->STR_SS_TCP_MAILBOX_POPPORT,
	End;

	Object->STR_SS_TCP_MAILBOX_SMTPHOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_SMTPHOST",
	End;

	Object->STR_SS_TCP_MAILBOX_SMTPPORT = StringObject,
		MUIA_Weight, 20,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_SMTPPORT",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 7,
	End;

	GR_SS_TCP_MAILBOX_SMTPHOST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_SMTPHOST",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_SMTPHOST,
		Child, Label("SMTPPort"),
		Child, Object->STR_SS_TCP_MAILBOX_SMTPPORT,
	End;

	Object->CH_SS_TCP_MAILBOX_8BIT = CheckMark(FALSE);

	Object->CH_SS_TCP_MAILBOX_APOP = CheckMark(FALSE);

	Object->CH_SS_TCP_MAILBOX_EXTSERVER = CheckMark(FALSE);

	GR_SS_TCP_MAILBOX_FLAGS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_FLAGS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_SS_TCP_MAILBOX_8BIT,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Use APOP"),
			Child, Object->CH_SS_TCP_MAILBOX_APOP,
		End,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("External Server"),
			Child, Object->CH_SS_TCP_MAILBOX_EXTSERVER,
		End,
	End;

	Object->STR_SS_TCP_MAILBOX_INVALID_SUBJECT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_INVALID_SUBJECT",
		MUIA_String_Contents, "Message status - unknown User",
	End;

	Object->PA_SS_TCP_MAILBOX_INVALID_TEXT = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_MAILBOX_INVALID_TEXT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_MAILBOX_INVALID_TEXT = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_MAILBOX_INVALID_TEXT = PopButton(MUII_PopFile),
	End;

	Object->STR_SS_TCP_MAILBOX_INVALID_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_INVALID_NAME",
		MUIA_String_Contents, "MAILER-DAEMON",
	End;

	Object->TX_SS_TCP_MAILBOX_INVALID_NAME = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_TCP_MAILBOX_INVALID_NAME,
		MUIA_Text_SetMin, TRUE,
	End;

	GR_SS_TCP_MAILBOX_INVALID_NAME = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_INVALID_NAME",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_INVALID_NAME,
		Child, Label("@"),
		Child, Object->TX_SS_TCP_MAILBOX_INVALID_NAME,
	End;

	Object->PA_SS_TCP_MAILBOX_INBOUND = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_MAILBOX_INBOUND",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_MAILBOX_INBOUND = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_MAILBOX_INBOUND = PopButton(MUII_PopDrawer),
	End;

	Object->PA_SS_TCP_MAILBOX_OUTBOUND = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_MAILBOX_OUTBOUND",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_MAILBOX_OUTBOUND = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_MAILBOX_OUTBOUND = PopButton(MUII_PopDrawer),
	End;

	GR_SS_TCP_MAILBOX_HOSTS_PARA = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_HOSTS_PARA",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "POP3 Mailbox Settings",
		MUIA_Group_Columns, 2,
		Child, Label("Name"),
		Child, Object->STR_SS_TCP_MAILBOX_NAME,
		Child, Label("Domain"),
		Child, Object->STR_SS_TCP_MAILBOX_DOMAIN,
		Child, Label("POP3 Hostname"),
		Child, GR_SS_TCP_MAILBOX_POPHOST,
		Child, Label("SMTP Hostname"),
		Child, GR_SS_TCP_MAILBOX_SMTPHOST,
		Child, Label("Allow 8Bit"),
		Child, GR_SS_TCP_MAILBOX_FLAGS,
		Child, Label("Invalid Mail Subject"),
		Child, Object->STR_SS_TCP_MAILBOX_INVALID_SUBJECT,
		Child, Label("Invalid Mail Text (Filename)"),
		Child, Object->PA_SS_TCP_MAILBOX_INVALID_TEXT,
		Child, Label("Invalid Name (From:)"),
		Child, GR_SS_TCP_MAILBOX_INVALID_NAME,
		Child, Label("Inbound Path"),
		Child, Object->PA_SS_TCP_MAILBOX_INBOUND,
		Child, Label("Outbound Path"),
		Child, Object->PA_SS_TCP_MAILBOX_OUTBOUND,
	End;

	Object->LV_SS_TCP_MAILBOX_USERS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListMailserverUserHook,
	End;

	Object->LV_SS_TCP_MAILBOX_USERS = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_MAILBOX_USERS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_TCP_MAILBOX_USERS,
	End;

	Object->BT_SS_TCP_MAILBOX_USERS_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_MAILBOX_USERS_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_TCP_MAILBOX_USERS_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_MAILBOX_USERS_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_TCP_MAILBOX_USERS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_TCP_MAILBOX_USERS_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_TCP_MAILBOX_USERS_REMOVE,
	End;

	GR_SS_TCP_MAILBOX_USERLIST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERLIST",
		MUIA_Weight, 50,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Users",
		Child, Object->LV_SS_TCP_MAILBOX_USERS,
		Child, GR_SS_TCP_MAILBOX_USERS_ACTION,
	End;

	Object->LV_SS_TCP_MAILBOX_USERS_NAME = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListudataHook,
	End;

	Object->LV_SS_TCP_MAILBOX_USERS_NAME = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_MAILBOX_USERS_NAME",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_TCP_MAILBOX_USERS_NAME,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_NAME",
	End;

	Object->GR_SS_TCP_MAILBOX_USERS_NAME = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_NAME",
		Child, Object->LV_SS_TCP_MAILBOX_USERS_NAME,
		Child, Object->STR_SS_TCP_MAILBOX_USERS_NAME,
	End;

	Object->PO_SS_TCP_MAILBOX_USERS_NAME = PopobjectObject,
		MUIA_HelpNode, "PO_SS_TCP_MAILBOX_USERS_NAME",
		MUIA_Popstring_String, Object->STR_PO_SS_TCP_MAILBOX_USERS_NAME = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->GR_SS_TCP_MAILBOX_USERS_NAME,
	End;

	Object->CY_SS_TCP_USERS_TYPE = CycleObject,
		MUIA_HelpNode, "CY_SS_TCP_USERS_TYPE",
		MUIA_Cycle_Entries, Object->CY_SS_TCP_USERS_TYPEContent,
	End;

	GR_SS_TCP_MAILBOX_USERS_TYPE = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_TYPE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CY_SS_TCP_USERS_TYPE,
		Child, HVSpace,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_POPID = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_POPID",
	End;

	Object->CH_SS_TCP_MAILBOX_USERS_POPID_FROMBBS = CheckMark(FALSE);

	GR_SS_TCP_MAILBOX_USERS_POPID = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_POPID",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_USERS_POPID,
		Child, Label("Use BBS-Username"),
		Child, Object->CH_SS_TCP_MAILBOX_USERS_POPID_FROMBBS,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_PW = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_PW",
	End;

	Object->CH_SS_TCP_MAILBOX_USERS_PW_FROMBBS = CheckMark(FALSE);

	GR_SS_TCP_MAILBOX_USERS_PW = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_PW",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_USERS_PW,
		Child, Label("Use BBS-Password"),
		Child, Object->CH_SS_TCP_MAILBOX_USERS_PW_FROMBBS,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_HOLDMAILS = StringObject,
		MUIA_Weight, 20,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_HOLDMAILS",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 5,
	End;

	GR_SS_TCP_MAILBOX_USERS_HOLDMAILS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_HOLDMAILS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_USERS_HOLDMAILS,
		Child, HVSpace,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_MAXMAILS = StringObject,
		MUIA_Weight, 20,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_MAXMAILS",
		MUIA_String_Accept, "-0123456789",
	End;

	GR_SS_TCP_MAILBOX_USERS_MAXMAILS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_MAXMAILS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_MAILBOX_USERS_MAXMAILS,
		Child, HVSpace,
	End;

	Object->STR_SS_TCP_MAILBOX_USERS_SPACE = StringObject,
		MUIA_Weight, 20,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_MAILBOX_USERS_SPACE",
		MUIA_String_Accept, "-0123456789",
	End;

	GR_SS_TCP_MAILBOX_USER_PARA = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USER_PARA",
		MUIA_Group_Columns, 2,
		Child, Label("Username"),
		Child, Object->PO_SS_TCP_MAILBOX_USERS_NAME,
		Child, Label("Type"),
		Child, GR_SS_TCP_MAILBOX_USERS_TYPE,
		Child, Label("POP3 ID"),
		Child, GR_SS_TCP_MAILBOX_USERS_POPID,
		Child, Label("POP3 Password"),
		Child, GR_SS_TCP_MAILBOX_USERS_PW,
		Child, Label("Hold Mail (Days)"),
		Child, GR_SS_TCP_MAILBOX_USERS_HOLDMAILS,
		Child, Label("Maximum Mails"),
		Child, GR_SS_TCP_MAILBOX_USERS_MAXMAILS,
		Child, Label("Disk Space (MByte)"),
		Child, GroupObject,
			MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_SPACE",
			MUIA_Group_Horiz, TRUE,
			Child, Object->STR_SS_TCP_MAILBOX_USERS_SPACE,
			Child, HVSpace,
		End,
		Child, HVSpace,
	End;

	Object->GR_SS_TCP_MAILBOX_USERS_PARAMETERS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS_PARAMETERS",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "User Settings",
		Child, HVSpace,
		Child, GR_SS_TCP_MAILBOX_USER_PARA,
		Child, HVSpace,
	End;

	Object->GR_SS_TCP_MAILBOX_USERS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_USERS",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_TCP_MAILBOX_USERLIST,
		Child, Object->GR_SS_TCP_MAILBOX_USERS_PARAMETERS,
	End;

	Object->GR_SS_TCP_MAILBOX_PARAMETERS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX_PARAMETERS",
		MUIA_Disabled, TRUE,
		Child, GR_SS_TCP_MAILBOX_HOSTS_PARA,
		Child, Object->GR_SS_TCP_MAILBOX_USERS,
	End;

	GR_SS_TCP_MAILBOX = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_MAILBOX",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_TCP_MAILBOX_HOSTLIST,
		Child, Object->GR_SS_TCP_MAILBOX_PARAMETERS,
	End;

	Object->LV_SS_TCP_POP_HOSTS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListPOP3ServerHook,
	End;

	Object->LV_SS_TCP_POP_HOSTS = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_POP_HOSTS",
		MUIA_Listview_List, Object->LV_SS_TCP_POP_HOSTS,
	End;

	Object->BT_SS_TCP_POP_HOSTS_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_POP_HOSTS_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_TCP_POP_HOSTS_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_POP_HOSTS_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_TCP_POP_HOSTS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_HOSTS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_TCP_POP_HOSTS_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_TCP_POP_HOSTS_REMOVE,
	End;

	GR_SS_TCP_POP_HOSTLIST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_HOSTLIST",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Server",
		Child, Object->LV_SS_TCP_POP_HOSTS,
		Child, GR_SS_TCP_POP_HOSTS_ACTION,
	End;

	Object->STR_SS_TCP_POP_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_NAME",
	End;

	Object->STR_SS_TCP_POP_DOMAIN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_DOMAIN",
		MUIA_String_MaxLen, 120,
	End;

	Object->STR_SS_TCP_POP_SMTPHOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_SMTPHOST",
	End;

	Object->STR_SS_TCP_POP_POPHOST = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_POPHOST",
	End;

	Object->STR_SS_TCP_POP_USERNAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_USERNAME",
	End;

	Object->STR_SS_TCP_POP_PASSWORD = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_PASSWORD",
	End;

	Object->CH_SS_TCP_POP_8BIT = CheckMark(FALSE);

	Object->CH_SS_TCP_POP_APOP = CheckMark(FALSE);

	Object->CH_SS_TCP_POP_DELMAILS = CheckMark(FALSE);

	Object->CH_SS_TCP_POP_EXTSERVER = CheckMark(FALSE);

	GR_SS_TCP_POP_FLAGS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_FLAGS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_SS_TCP_POP_8BIT,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Use APOP"),
			Child, Object->CH_SS_TCP_POP_APOP,
		End,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Delete Mails on Server"),
			Child, Object->CH_SS_TCP_POP_DELMAILS,
		End,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("External Server"),
			Child, Object->CH_SS_TCP_POP_EXTSERVER,
		End,
	End;

	Object->STR_SS_TCP_POP_INVALID_SUBJECT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_INVALID_SUBJECT",
		MUIA_String_Contents, "Message status - unknown User",
	End;

	Object->PA_SS_TCP_POP_INVALID_TEXT = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_POP_INVALID_TEXT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_POP_INVALID_TEXT = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_POP_INVALID_TEXT = PopButton(MUII_PopFile),
	End;

	Object->STR_SS_TCP_POP_INVALID_NAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_TCP_POP_INVALID_NAME",
		MUIA_String_Contents, "MAILER-DAEMON",
	End;

	Object->TX_SS_TCP_POP_INVALID_NAME = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_TCP_POP_INVALID_NAME,
		MUIA_Text_SetMin, TRUE,
	End;

	GR_SS_TCP_POP_INVALID_NAME = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_INVALID_NAME",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_TCP_POP_INVALID_NAME,
		Child, Label("@"),
		Child, Object->TX_SS_TCP_POP_INVALID_NAME,
	End;

	Object->PA_SS_TCP_POP_INBOUND = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_POP_INBOUND",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_POP_INBOUND = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_POP_INBOUND = PopButton(MUII_PopDrawer),
	End;

	Object->PA_SS_TCP_POP_OUTBOUND = PopaslObject,
		MUIA_HelpNode, "PA_SS_TCP_POP_OUTBOUND",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_SS_TCP_POP_OUTBOUND = String("", 80),
		MUIA_Popstring_Button, Object->PA_SS_TCP_POP_OUTBOUND = PopButton(MUII_PopDrawer),
	End;

	Object->GR_SS_TCP_POP_PARA = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_PARA",
		MUIA_Disabled, TRUE,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "SMTP/POP3 Settings",
		MUIA_Group_Columns, 2,
		Child, Label("Server Name"),
		Child, Object->STR_SS_TCP_POP_NAME,
		Child, Label("Domain"),
		Child, Object->STR_SS_TCP_POP_DOMAIN,
		Child, Label("SMTP Hostname"),
		Child, Object->STR_SS_TCP_POP_SMTPHOST,
		Child, Label("POP3 Hostname"),
		Child, Object->STR_SS_TCP_POP_POPHOST,
		Child, Label("POP3 Username"),
		Child, Object->STR_SS_TCP_POP_USERNAME,
		Child, Label("POP3 Password"),
		Child, Object->STR_SS_TCP_POP_PASSWORD,
		Child, Label("Use 8Bit"),
		Child, GR_SS_TCP_POP_FLAGS,
		Child, Label("Invalid Mail Subject"),
		Child, Object->STR_SS_TCP_POP_INVALID_SUBJECT,
		Child, Label("Invalid Mail Textpath"),
		Child, Object->PA_SS_TCP_POP_INVALID_TEXT,
		Child, Label("Invalid Name (From:)"),
		Child, GR_SS_TCP_POP_INVALID_NAME,
		Child, Label("Inbound Path"),
		Child, Object->PA_SS_TCP_POP_INBOUND,
		Child, Label("Outbound Path"),
		Child, Object->PA_SS_TCP_POP_OUTBOUND,
	End;

	GR_SS_TCP_POP_PARAMETERS = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP_PARAMETERS",
		Child, HVSpace,
		Child, Object->GR_SS_TCP_POP_PARA,
		Child, HVSpace,
	End;

	GR_SS_TCP_POP = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_POP",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_TCP_POP_HOSTLIST,
		Child, GR_SS_TCP_POP_PARAMETERS,
	End;

	Object->LV_SS_TCP_USERS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_TCP_USERS = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_USERS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_TCP_USERS,
	End;

	Object->BT_SS_TCP_USERS_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_USERS_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_TCP_USERS_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_USERS_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_TCP_USERS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_TCP_USERS_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_TCP_USERS_REMOVE,
	End;

	GR_SS_TCP_USERLIST = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERLIST",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "User",
		Child, Object->LV_SS_TCP_USERS,
		Child, GR_SS_TCP_USERS_ACTION,
	End;

	Object->LV_SS_TCP_USERS_IN_SERVER = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_TCP_USERS_IN_SERVER = ListviewObject,
		MUIA_HelpNode, "LV_SS_TCP_USERS_IN_SERVER",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_TCP_USERS_IN_SERVER,
	End;

	Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPE = CycleObject,
		MUIA_HelpNode, "CY_SS_TCP_USERS_IN_SEARCH_NAMETYPE",
		MUIA_Cycle_Entries, Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPEContent,
	End;

	Object->CY_SS_TCP_USERS_IN_SEARCH = CycleObject,
		MUIA_HelpNode, "CY_SS_TCP_USERS_IN_SEARCH",
		MUIA_Cycle_Entries, Object->CY_SS_TCP_USERS_IN_SEARCHContent,
	End;

	GR_SS_TCP_USERS_IN_SEARCH = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_IN_SEARCH",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPE,
		Child, Label("in "),
		Child, Object->CY_SS_TCP_USERS_IN_SEARCH,
		Child, HVSpace,
		Child, Label("Search"),
	End;

	Object->LV_label_25 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_label_25 = ListviewObject,
		MUIA_HelpNode, "LV_label_25",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_label_25,
	End;

	Object->PO_SS_USERS_OUT_MOVETO = PopobjectObject,
		MUIA_HelpNode, "PO_SS_USERS_OUT_MOVETO",
		MUIA_Popstring_String, Object->STR_PO_SS_USERS_OUT_MOVETO = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_label_25,
	End;

	Object->STR_SS_USERS_OUT_FROM = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_USERS_OUT_FROM",
	End;

	Object->TX_SS_USERS_OUT_FROM = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_SS_USERS_OUT_FROM,
		MUIA_Text_SetMin, TRUE,
	End;

	GR_SS_USERS_OUT_FROM = GroupObject,
		MUIA_HelpNode, "GR_SS_USERS_OUT_FROM",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_USERS_OUT_FROM,
		Child, Label("From:"),
		Child, Object->TX_SS_USERS_OUT_FROM,
	End;

	Object->STR_SS_USERS_OUT_REPLYTO = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_USERS_OUT_REPLYTO",
	End;

	Object->STR_SS_USERS_OUT_REPLYTO_DOMAIN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_USERS_OUT_REPLYTO_DOMAIN",
	End;

	GR_SS_USERS_OUT_REPLYTO = GroupObject,
		MUIA_HelpNode, "GR_SS_USERS_OUT_REPLYTO",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_SS_USERS_OUT_REPLYTO,
		Child, Label("@"),
		Child, Object->STR_SS_USERS_OUT_REPLYTO_DOMAIN,
	End;

	GR_SS_TCP_USERS_OUT = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_OUT",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Outgoing",
		MUIA_Group_Columns, 2,
		Child, Label("Move To"),
		Child, Object->PO_SS_USERS_OUT_MOVETO,
		Child, Label("From:"),
		Child, GR_SS_USERS_OUT_FROM,
		Child, Label("Reply To:"),
		Child, GR_SS_USERS_OUT_REPLYTO,
	End;

	Object->BT_SS_TCP_USERS_IN_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_USERS_IN_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_TCP_USERS_IN_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_TCP_USERS_IN_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_TCP_USERS_USERS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_USERS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_TCP_USERS_IN_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_TCP_USERS_IN_REMOVE,
	End;

	GR_SS_TCP_USERS_IN_PARA = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_IN_PARA",
		Child, HSpace(0),
		Child, GR_SS_TCP_USERS_IN_SEARCH,
		Child, HVSpace,
		Child, GR_SS_TCP_USERS_OUT,
		Child, GR_SS_TCP_USERS_USERS_ACTION,
	End;

	GR_SS_TCP_USERS_IN = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_USERS_IN",
		MUIA_Weight, 50,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Incomming",
		MUIA_Group_Horiz, TRUE,
		Child, Object->LV_SS_TCP_USERS_IN_SERVER,
		Child, GR_SS_TCP_USERS_IN_PARA,
	End;

	GR_SS_TCP_ROUTING = GroupObject,
		MUIA_HelpNode, "GR_SS_TCP_ROUTING",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_TCP_USERLIST,
		Child, GR_SS_TCP_USERS_IN,
	End;

	GR_SS_TCPIP = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_SS_TCPIP,
		MUIA_HelpNode, "GR_SS_TCPIP",
		Child, GR_SS_TCP_MISC,
		Child, GR_SS_TCP_MAILBOX,
		Child, GR_SS_TCP_POP,
		Child, GR_SS_TCP_ROUTING,
	End;

	Object->LV_label_6 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_label_6 = ListviewObject,
		MUIA_HelpNode, "LV_label_6",
		MUIA_Listview_List, Object->LV_label_6,
	End;

	Object->BT_SS_FIDONEW = SimpleButton("New");

	Object->BT_SS_FIDOADD = SimpleButton("Add");

	Object->BT_SS_FIDOREMOVE = SimpleButton("Remove");

	GR_SS_FIDOACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_FIDONEW,
		Child, Object->BT_SS_FIDOADD,
		Child, Object->BT_SS_FIDOREMOVE,
	End;

	GR_SS_FIDOMISC = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOMISC",
		MUIA_Weight, 20,
		Child, Object->LV_label_6,
		Child, GR_SS_FIDOACTION,
	End;

	Object->STR_SS_FIDOFIRSTZONEC = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOFIRSTZONEC",
	End;

	Object->STR_SS_FIDOLASTZONE = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOLASTZONE",
	End;

	Object->STR_SS_FIDOFAKENET = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOFAKENET",
	End;

	Object->STR_SS_ADDRESS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ADDRESS",
	End;

	Object->STR_SS_FIDOFIRSTAKA = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOFIRSTAKA",
	End;

	Object->STR_SS_FIDOSECONDAKA = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOSECONDAKA",
	End;

	Object->STR_SS_FIDOECHOORIGIN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOECHOORIGIN",
	End;

	Object->STR_SS_FIDONETWORKORIGIN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDONETWORKORIGIN",
	End;

	GR_SS_FIDONETWORK = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDONETWORK",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("First Zone"),
		Child, Object->STR_SS_FIDOFIRSTZONEC,
		Child, Label("Last Zone"),
		Child, Object->STR_SS_FIDOLASTZONE,
		Child, Label("Fake/Point-net"),
		Child, Object->STR_SS_FIDOFAKENET,
		Child, Label("Address (0)"),
		Child, Object->STR_SS_ADDRESS,
		Child, Label("First AKA (1)"),
		Child, Object->STR_SS_FIDOFIRSTAKA,
		Child, Label("Second AKA (2)"),
		Child, Object->STR_SS_FIDOSECONDAKA,
		Child, Label("Echo origin"),
		Child, Object->STR_SS_FIDOECHOORIGIN,
		Child, Label("Netware origin"),
		Child, Object->STR_SS_FIDONETWORKORIGIN,
	End;

	Object->LV_SS_FIDOAREAS_AREASLIST = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_FIDOAREAS_AREASLIST = ListviewObject,
		MUIA_HelpNode, "LV_SS_FIDOAREAS_AREASLIST",
		MUIA_Listview_List, Object->LV_SS_FIDOAREAS_AREASLIST,
	End;

	Object->STR_SS_FIDOAREAS_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOAREAS_INPUT",
	End;

	Object->BT_SS_FIDOAREAS_NEW = SimpleButton("New");

	Object->BT_SS_FIDOAREAS_ADD = SimpleButton("Add");

	Object->BT_SS_FIDOAREAS_REMOVE = SimpleButton("Remove");

	GR_SS_FIDOAREAS_AREAS = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOAREAS_AREAS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_FIDOAREAS_NEW,
		Child, Object->BT_SS_FIDOAREAS_ADD,
		Child, Object->BT_SS_FIDOAREAS_REMOVE,
	End;

	GR_SS_FIDOMAREAS_AREAS = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOMAREAS_AREAS",
		MUIA_Weight, 50,
		MUIA_FrameTitle, "Areas",
		Child, Object->LV_SS_FIDOAREAS_AREASLIST,
		Child, Object->STR_SS_FIDOAREAS_INPUT,
		Child, GR_SS_FIDOAREAS_AREAS,
	End;

	Object->LV_FIDOAREAS_EXPORT = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_FIDOAREAS_EXPORT = ListviewObject,
		MUIA_HelpNode, "LV_FIDOAREAS_EXPORT",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_FIDOAREAS_EXPORT,
	End;

	Object->STR_FIDOAREAS_ACCESSLEVEL = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOAREAS_ACCESSLEVEL",
	End;

	Object->STR_FIDOAREAS_ACCESSFLAGS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOAREAS_ACCESSFLAGS",
	End;

	Object->STR_FIDOAREAS_DUPETABLE = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOAREAS_DUPETABLE",
	End;

	Object->STR_FIDOAREAS_DESCRIPTION = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOAREAS_DESCRIPTION",
	End;

	GR_FIDOAREAS_AREAS = GroupObject,
		MUIA_HelpNode, "GR_FIDOAREAS_AREAS",
		MUIA_FrameTitle, "Access/Flags",
		MUIA_Group_Columns, 2,
		Child, Label("Access level"),
		Child, Object->STR_FIDOAREAS_ACCESSLEVEL,
		Child, Label("Access flags"),
		Child, Object->STR_FIDOAREAS_ACCESSFLAGS,
		Child, Label("Dupe table"),
		Child, Object->STR_FIDOAREAS_DUPETABLE,
		Child, Label("Discription"),
		Child, Object->STR_FIDOAREAS_DESCRIPTION,
	End;

	GR_FIDOAREAS_FLAGS = GroupObject,
		MUIA_HelpNode, "GR_FIDOAREAS_FLAGS",
		MUIA_FrameTitle, "Export to",
		Child, Object->LV_FIDOAREAS_EXPORT,
		Child, GR_FIDOAREAS_AREAS,
	End;

	GR_SS_FIDOAREAS = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOAREAS",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_SameWidth, TRUE,
		Child, GR_SS_FIDOMAREAS_AREAS,
		Child, GR_FIDOAREAS_FLAGS,
	End;

	Object->LV_SS_FIDOEXPORT_EXPORTTO = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_FIDOEXPORT_EXPORTTO = ListviewObject,
		MUIA_HelpNode, "LV_SS_FIDOEXPORT_EXPORTTO",
		MUIA_Listview_List, Object->LV_SS_FIDOEXPORT_EXPORTTO,
	End;

	Object->STR_SS_FIDOEXPORT_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOEXPORT_INPUT",
	End;

	Object->BT_SS_FIDOEXPORT_NEW = SimpleButton("New");

	Object->BT_SS_FIDOEXPORT_ADD = SimpleButton("Add");

	Object->BT_SS_FIDOEXPORT_REMOVE = SimpleButton("Remove");

	GR_SS_FIDOEXPORT_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOEXPORT_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_FIDOEXPORT_NEW,
		Child, Object->BT_SS_FIDOEXPORT_ADD,
		Child, Object->BT_SS_FIDOEXPORT_REMOVE,
	End;

	GR_SS_FIDOEXPORT_EXPORT = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOEXPORT_EXPORT",
		MUIA_Weight, 50,
		MUIA_FrameTitle, "Areas",
		Child, Object->LV_SS_FIDOEXPORT_EXPORTTO,
		Child, Object->STR_SS_FIDOEXPORT_INPUT,
		Child, GR_SS_FIDOEXPORT_ACTION,
	End;

	Object->LV_FIDOEXPORT_AREAS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_FIDOEXPORT_AREAS = ListviewObject,
		MUIA_HelpNode, "LV_FIDOEXPORT_AREAS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_FIDOEXPORT_AREAS,
	End;

	Object->CY_FIDOEXPORT_ARC = CycleObject,
		MUIA_HelpNode, "CY_FIDOEXPORT_ARC",
		MUIA_Cycle_Entries, Object->CY_FIDOEXPORT_ARCContent,
	End;

	Object->RA_FIDOEXPORT_MAILTYPE = RadioObject,
		MUIA_Weight, 0,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_HelpNode, "RA_FIDOEXPORT_MAILTYPE",
		MUIA_Radio_Entries, Object->RA_FIDOEXPORT_MAILTYPEContent,
	End;

	Object->RA_CC_STARTUPMODECC = RadioObject,
		MUIA_Weight, 0,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_HelpNode, "RA_CC_STARTUPMODECC",
		MUIA_Radio_Entries, Object->RA_CC_STARTUPMODECCContent,
	End;

	GR_grp_43CC = GroupObject,
		MUIA_HelpNode, "GR_grp_43CC",
		MUIA_Group_Horiz, TRUE,
		Child, Object->RA_FIDOEXPORT_MAILTYPE,
		Child, HSpace(0),
		Child, Label("Dimension"),
		Child, Object->RA_CC_STARTUPMODECC,
	End;

	Object->STR_FIDOEXPORT_ACCESSLEVEL = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOEXPORT_ACCESSLEVEL",
	End;

	Object->STR_FIDOEXPORT_ACCESSFLAGS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOEXPORT_ACCESSFLAGS",
	End;

	Object->STR_FIDOEXPORT_AFPASSWORD = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOEXPORT_AFPASSWORD",
	End;

	GR_FIDOEXPORT_OPTIONS = GroupObject,
		MUIA_HelpNode, "GR_FIDOEXPORT_OPTIONS",
		MUIA_FrameTitle, "Access/Flags",
		MUIA_Group_Columns, 2,
		Child, Label("Archiver"),
		Child, Object->CY_FIDOEXPORT_ARC,
		Child, Label("Mail type"),
		Child, GR_grp_43CC,
		Child, Label("Description"),
		Child, Object->STR_FIDOEXPORT_ACCESSLEVEL,
		Child, Label("Access flags"),
		Child, Object->STR_FIDOEXPORT_ACCESSFLAGS,
		Child, Label("AF Password"),
		Child, Object->STR_FIDOEXPORT_AFPASSWORD,
	End;

	GR_FIDOEXPORT_FLAGS = GroupObject,
		MUIA_HelpNode, "GR_FIDOEXPORT_FLAGS",
		MUIA_FrameTitle, "Export to",
		Child, Object->LV_FIDOEXPORT_AREAS,
		Child, GR_FIDOEXPORT_OPTIONS,
	End;

	GR_SS_FIDOEXPORT = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOEXPORT",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_SameWidth, TRUE,
		Child, GR_SS_FIDOEXPORT_EXPORT,
		Child, GR_FIDOEXPORT_FLAGS,
	End;

	Object->LV_SS_FIDOROUTING_ROUTE = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_FIDOROUTING_ROUTE = ListviewObject,
		MUIA_HelpNode, "LV_SS_FIDOROUTING_ROUTE",
		MUIA_Listview_List, Object->LV_SS_FIDOROUTING_ROUTE,
	End;

	Object->STR_SS_FIDOROUTING_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_FIDOROUTING_INPUT",
	End;

	Object->BT_SS_FIDOROUTING_INSERT = SimpleButton("Insert");

	Object->BT_SS_FIDOROUTING_NEW = SimpleButton("New");

	Object->BT_SS_FIDOROUTING_ADD = SimpleButton("Add");

	Object->BT_SS_FIDOROUTING_REMOVE = SimpleButton("Remove");

	GR_SS_FIDOROUTING_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOROUTING_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_FIDOROUTING_INSERT,
		Child, Object->BT_SS_FIDOROUTING_NEW,
		Child, Object->BT_SS_FIDOROUTING_ADD,
		Child, Object->BT_SS_FIDOROUTING_REMOVE,
	End;

	GR_SS_FIDOMAREAS_AREASC = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOMAREAS_AREASC",
		MUIA_Weight, 50,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Route mail",
		Child, Object->LV_SS_FIDOROUTING_ROUTE,
		Child, Object->STR_SS_FIDOROUTING_INPUT,
		Child, GR_SS_FIDOROUTING_ACTION,
	End;

	Object->STR_FIDOROUTING_ROUTETO = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_FIDOROUTING_ROUTETO",
	End;

	Object->RA_FIDOROUTING_MAILTYPEC = RadioObject,
		MUIA_Weight, 0,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_HelpNode, "RA_FIDOROUTING_MAILTYPEC",
		MUIA_Radio_Entries, Object->RA_FIDOROUTING_MAILTYPECContent,
	End;

	GR_SS_FIDOROUTING_MAILTYPE = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOROUTING_MAILTYPE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->RA_FIDOROUTING_MAILTYPEC,
		Child, HSpace(0),
	End;

	GR_FIDOROUTING_OPTIONS = GroupObject,
		MUIA_HelpNode, "GR_FIDOROUTING_OPTIONS",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("Route to"),
		Child, Object->STR_FIDOROUTING_ROUTETO,
		Child, Label("Mail type"),
		Child, GR_SS_FIDOROUTING_MAILTYPE,
		Child, HVSpace,
		Child, HVSpace,
	End;

	GR_SS_FIDOMAILROUTING = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDOMAILROUTING",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_FIDOMAREAS_AREASC,
		Child, GR_FIDOROUTING_OPTIONS,
	End;

	GR_SS_FIDOOPTIONS = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_SS_FIDOOPTIONS,
		MUIA_HelpNode, "GR_SS_FIDOOPTIONS",
		MUIA_Frame, MUIV_Frame_Group,
		Child, GR_SS_FIDONETWORK,
		Child, GR_SS_FIDOAREAS,
		Child, GR_SS_FIDOEXPORT,
		Child, GR_SS_FIDOMAILROUTING,
	End;

	GR_SS_FIDO = GroupObject,
		MUIA_HelpNode, "GR_SS_FIDO",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_FIDOMISC,
		Child, GR_SS_FIDOOPTIONS,
	End;

	GR_SS_Network = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_SS_Network,
		MUIA_HelpNode, "GR_SS_Network",
		Child, GR_SS_TCPIP,
		Child, GR_SS_FIDO,
	End;

	Object->CY_SS_Client_Sort = CycleObject,
		MUIA_HelpNode, "CY_SS_Client_Sort",
		MUIA_Weight, 0,
		MUIA_Cycle_Entries, Object->CY_SS_Client_SortContent,
	End;

	Object->STR_SS_Clients_Filter = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_Clients_Filter",
		MUIA_String_MaxLen, 40,
	End;

	GR_grp_26 = GroupObject,
		MUIA_HelpNode, "GR_grp_26",
		MUIA_Group_Horiz, TRUE,
		Child, Label("Sorted by"),
		Child, Object->CY_SS_Client_Sort,
		Child, HVSpace,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Filter"),
			Child, Object->STR_SS_Clients_Filter,
		End,
	End;

	Object->LV_SS_Clients = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListcportminHook,
	End;

	Object->LV_SS_Clients = ListviewObject,
		MUIA_HelpNode, "LV_SS_Clients",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, Object->LV_SS_Clients,
	End;

	Object->BT_SS_Clients_New = TextObject,
		ButtonFrame,
		MUIA_Weight, 1,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Clients_New",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_Clients_Edit = TextObject,
		ButtonFrame,
		MUIA_Weight, 1,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Edit",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Clients_Edit",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_Clients_Remove = TextObject,
		ButtonFrame,
		MUIA_Weight, 1,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Clients_Remove",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_grp_6 = GroupObject,
		MUIA_HelpNode, "GR_grp_6",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_SameWidth, TRUE,
		Child, Object->BT_SS_Clients_New,
		Child, Object->BT_SS_Clients_Edit,
		Child, Object->BT_SS_Clients_Remove,
	End;

	GR_grp_8 = GroupObject,
		MUIA_HelpNode, "GR_grp_8",
		Child, GR_grp_26,
		Child, Object->LV_SS_Clients,
		Child, GR_grp_6,
	End;

	GR_SS_Clients = GroupObject,
		MUIA_HelpNode, "GR_SS_Clients",
		Child, GR_grp_8,
	End;

	Object->CY_label_3CC = CycleObject,
		MUIA_HelpNode, "CY_label_3CC",
		MUIA_Weight, 0,
		MUIA_Cycle_Entries, Object->CY_label_3CCContent,
	End;

	Object->STR_SS_Clients_FilterCC = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_Clients_FilterCC",
		MUIA_String_MaxLen, 40,
	End;

	GR_grp_26CC = GroupObject,
		MUIA_HelpNode, "GR_grp_26CC",
		MUIA_Group_Horiz, TRUE,
		Child, Label("Sorted by"),
		Child, Object->CY_label_3CC,
		Child, HVSpace,
			Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Filter"),
			Child, Object->STR_SS_Clients_FilterCC,
		End,
	End;

	Object->LV_SS_Users = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_DisplayHook, &DisplayListudataHook,
	End;

	Object->LV_SS_Users = ListviewObject,
		MUIA_HelpNode, "LV_SS_Users",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, Object->LV_SS_Users,
	End;

	Object->BT_SS_Users_New = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Users_New",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_Users_Edit = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Edit",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Users_Edit",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_Users_Remove = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Users_Remove",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_Users_NewList = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Create New List",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_Users_NewList",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_grp_6CC = GroupObject,
		MUIA_HelpNode, "GR_grp_6CC",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_SameWidth, TRUE,
		Child, Object->BT_SS_Users_New,
		Child, Object->BT_SS_Users_Edit,
		Child, Object->BT_SS_Users_Remove,
		Child, HVSpace,
		Child, Object->BT_SS_Users_NewList,
	End;

	GR_grp_8CC = GroupObject,
		MUIA_HelpNode, "GR_grp_8CC",
		Child, GR_grp_26CC,
		Child, Object->LV_SS_Users,
		Child, GR_grp_6CC,
	End;

	GR_SS_Users = GroupObject,
		MUIA_HelpNode, "GR_SS_Users",
		Child, GR_grp_8CC,
	End;

	Object->LV_label_8 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_label_8 = ListviewObject,
		MUIA_HelpNode, "LV_label_8",
		MUIA_Listview_List, Object->LV_label_8,
	End;

	Object->STR_SS_ARC_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_INPUT",
	End;

	Object->BT_SS_ARC_NEW = SimpleButton("New");

	Object->BT_SS_ARC_ADD = SimpleButton("Add");

	Object->BT_SS_ARC_REMOVE = SimpleButton("Remove");

	GR_grp_87 = GroupObject,
		MUIA_HelpNode, "GR_grp_87",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_ARC_NEW,
		Child, Object->BT_SS_ARC_ADD,
		Child, Object->BT_SS_ARC_REMOVE,
	End;

	GR_grp_85 = GroupObject,
		MUIA_HelpNode, "GR_grp_85",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Archivers",
		Child, Object->LV_label_8,
		Child, Object->STR_SS_ARC_INPUT,
		Child, GR_grp_87,
	End;

	Object->STR_SS_ARC_VIEWFORMAT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_VIEWFORMAT",
	End;

	Object->STR_SS_ARC_TESTFORMAT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_TESTFORMAT",
	End;

	Object->STR_SS_ARC_TESTKEYWORD = StringObject,
		MUIA_Weight, 40,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_TESTKEYWORD",
	End;

	Object->STR_SS_ARC_KEYWORDLINE = StringObject,
		MUIA_Weight, 10,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_KEYWORDLINE",
	End;

	Object->STR_SS_ARC_KEYWORDCOLOMN = StringObject,
		MUIA_Weight, 10,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_KEYWORDCOLOMN",
	End;

	Object->STR_SS_ARC_PACKFORMAT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_PACKFORMAT",
	End;

	Object->STR_SS_ARC_EXTRACTFORMAT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ARC_EXTRACTFORMAT",
	End;

	GR_grp_86 = GroupObject,
		MUIA_HelpNode, "GR_grp_86",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Parameters",
		MUIA_Group_Columns, 2,
		Child, HVSpace,
		Child, HVSpace,
		Child, Label("View format"),
		Child, Object->STR_SS_ARC_VIEWFORMAT,
		Child, Label("Test format"),
		Child, Object->STR_SS_ARC_TESTFORMAT,
		Child, Label("Test keyword"),
		Child, Object->STR_SS_ARC_TESTKEYWORD,
		Child, Label("Keyword line"),
		Child, Object->STR_SS_ARC_KEYWORDLINE,
		Child, Label("Keyword column"),
		Child, Object->STR_SS_ARC_KEYWORDCOLOMN,
		Child, Label("Pack format"),
		Child, Object->STR_SS_ARC_PACKFORMAT,
		Child, Label("Extract format"),
		Child, Object->STR_SS_ARC_EXTRACTFORMAT,
		Child, HVSpace,
		Child, HVSpace,
	End;

	GR_SS_Arc = GroupObject,
		MUIA_HelpNode, "GR_SS_Arc",
		MUIA_Group_Horiz, TRUE,
		Child, GR_grp_85,
		Child, GR_grp_86,
	End;

	Object->LV_SS_EDITORS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_EDITORS = ListviewObject,
		MUIA_HelpNode, "LV_SS_EDITORS",
		MUIA_Listview_List, Object->LV_SS_EDITORS,
	End;

	Object->STR_SS_EDITORS_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_EDITORS_INPUT",
	End;

	Object->BT_SS_EDITORS_NEW = SimpleButton("New");

	Object->BT_SS_EDITORS_ADD = SimpleButton("Add");

	Object->BT_SS_EDITORS_REMOVE = SimpleButton("Remove");

	GR_grp_87CC2 = GroupObject,
		MUIA_HelpNode, "GR_grp_87CC2",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_EDITORS_NEW,
		Child, Object->BT_SS_EDITORS_ADD,
		Child, Object->BT_SS_EDITORS_REMOVE,
	End;

	GR_grp_8we5CC = GroupObject,
		MUIA_HelpNode, "GR_grp_8we5CC",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Editors",
		Child, Object->LV_SS_EDITORS,
		Child, Object->STR_SS_EDITORS_INPUT,
		Child, GR_grp_87CC2,
	End;

	Object->STR_SS_EDITORS_PATH = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_EDITORS_PATH",
	End;

	Object->CY_label_10 = CycleObject,
		MUIA_HelpNode, "CY_label_10",
		MUIA_Cycle_Entries, Object->CY_label_10Content,
	End;

	GR_grp_88 = GroupObject,
		MUIA_HelpNode, "GR_grp_88",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Parameters",
		MUIA_Group_Columns, 3,
		MUIA_Group_SameSize, TRUE,
		Child, HVSpace,
		Child, HVSpace,
		Child, HVSpace,
		Child, Label("Path"),
		Child, Object->STR_SS_EDITORS_PATH,
		Child, HVSpace,
		Child, Label("Interface"),
		Child, Object->CY_label_10,
		Child, HVSpace,
		Child, HVSpace,
		Child, HVSpace,
		Child, HVSpace,
	End;

	GR_SS_Editors = GroupObject,
		MUIA_HelpNode, "GR_SS_Editors",
		MUIA_Group_Horiz, TRUE,
		Child, GR_grp_8we5CC,
		Child, GR_grp_88,
	End;

	Object->LV_SS_LOGS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_LOGS = ListviewObject,
		MUIA_HelpNode, "LV_SS_LOGS",
		MUIA_Listview_List, Object->LV_SS_LOGS,
	End;

	Object->STR_SS_LOGS_INPUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_LOGS_INPUT",
	End;

	Object->BT_SS_LOGS_NEW = SimpleButton("New");

	Object->BT_SS_LOGS_ADD = SimpleButton("Add");

	Object->BT_SS_LOGS_REMOVE = SimpleButton("Remove");

	GR_grp_87CC = GroupObject,
		MUIA_HelpNode, "GR_grp_87CC",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_LOGS_NEW,
		Child, Object->BT_SS_LOGS_ADD,
		Child, Object->BT_SS_LOGS_REMOVE,
	End;

	GR_grp_85CC = GroupObject,
		MUIA_HelpNode, "GR_grp_85CC",
		MUIA_Weight, 30,
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Logs",
		Child, Object->LV_SS_LOGS,
		Child, Object->STR_SS_LOGS_INPUT,
		Child, GR_grp_87CC,
	End;

	GR_SS_Logs = GroupObject,
		MUIA_HelpNode, "GR_SS_Logs",
		Child, GR_grp_85CC,
	End;

	Object->LV_SS_ACCESS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_SS_ACCESS = ListviewObject,
		MUIA_HelpNode, "LV_SS_ACCESS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_SS_ACCESS,
	End;

	Object->STR_SS_ACCESS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_SS_ACCESS",
	End;

	Object->BT_SS_ACCESS_NEW = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "New",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_ACCESS_NEW",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_SS_ACCESS_REMOVE = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_SS_ACCESS_REMOVE",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_SS_ACCESS_ACTION = GroupObject,
		MUIA_HelpNode, "GR_SS_ACCESS_ACTION",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_SS_ACCESS_NEW,
		Child, HVSpace,
		Child, Object->BT_SS_ACCESS_REMOVE,
	End;

	GR_SS_ACCESS_LIST = GroupObject,
		MUIA_HelpNode, "GR_SS_ACCESS_LIST",
		Child, Object->LV_SS_ACCESS,
		Child, Object->STR_SS_ACCESS,
		Child, GR_SS_ACCESS_ACTION,
	End;

	GR_SS_ACCESS_SETTINGS = GroupObject,
		MUIA_HelpNode, "GR_SS_ACCESS_SETTINGS",
	End;

	GR_SS_ACCESS = GroupObject,
		MUIA_HelpNode, "GR_SS_ACCESS",
		MUIA_Group_Horiz, TRUE,
		Child, GR_SS_ACCESS_LIST,
		Child, GR_SS_ACCESS_SETTINGS,
	End;

	GR_SS = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_SS,
		MUIA_HelpNode, "GR_SS",
		Child, GroupObject,
			MUIA_HelpNode, "GR_SS_Maintance",
			Child, GroupObject,
				MUIA_HelpNode, "GR_SS_MISC",
				MUIA_Group_Columns, 2,
				Child, Label("System Name"),
				Child, Object->STR_SystemName,
				Child, Label("Registration ID"),
				Child, Object->TX_RegistrationID,
				Child, Label("Registration Name"),
				Child, Object->TX_RegistrationName,
				Child, Label("Registration Location"),
				Child, Object->STR_RegLocation,
				Child, Label("MainPort Number"),
				Child, Object->STR_MainPortNumber,
				Child, Label("Default UUCP Domain"),
				Child, Object->STR_UUCPDomain,
				Child, RectangleObject,
					MUIA_Rectangle_HBar, TRUE,
					MUIA_FixHeight, 8,
				End,
				Child, RectangleObject,
					MUIA_Rectangle_HBar, TRUE,
					MUIA_FixHeight, 8,
				End,
				Child, Label("MainPath"),
				Child, Object->PA_SS_MAINPATH,
				Child, Label("UserDir"),
				Child, Object->PA_SS_USERDIR,
				Child, Label("SysData"),
				Child, Object->PA_SS_SYSDATA,
				Child, Label("SysText"),
				Child, Object->PA_SS_SYSTEXT,
				Child, Label("NewUser"),
				Child, Object->PA_SS_NEWUSER,
				Child, RectangleObject,
					MUIA_Rectangle_HBar, TRUE,
					MUIA_FixHeight, 8,
				End,
				Child, RectangleObject,
					MUIA_Rectangle_HBar, TRUE,
					MUIA_FixHeight, 8,
				End,
				Child, Label("Users"),
				Child, Object->TX_SS_Users,
				Child, Label("BaseDirs"),
				Child, Object->TX_SS_BASEDIRS,
				Child, Label("Number of Ports"),
				Child, Object->TX_SS_NUMBEROFPORTS,
			End,
		End,
		Child, GR_SS_Options,
		Child, GR_SS_Services,
		Child, GR_SS_Network,
		Child, GR_SS_Clients,
		Child, GR_SS_Users,
		Child, GR_SS_Arc,
		Child, GR_SS_Editors,
		Child, GR_SS_Logs,
		Child, GR_SS_ACCESS,
	End;

	Object->BT_SS_ACCEPT = SimpleButton("Accept");

	Object->BT_SS_CANCEL = SimpleButton("Cancel");

	Object->GR_WindowSystemService = GroupObject,
		MUIA_ShowMe, FALSE,
		Child, GR_SS,
		Child, GroupObject,
			MUIA_HelpNode, "GR_SS_ACTION",
			MUIA_Group_Horiz, TRUE,
			Child, Object->BT_SS_ACCEPT,
			Child, HVSpace,
			Child, Object->BT_SS_CANCEL,
		End,
	End;

	Object->WindowSystemService = WindowObject,
		MUIA_Window_Title, "SystemService",
		MUIA_Window_ID, MAKE_ID('2', 'W', 'I', 'N'),
		MUIA_Window_CloseGadget, FALSE,
		WindowContents, Object->GR_WindowSystemService,
	End;

	Object->TX_label_7 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_label_7,
	End;

	Object->STR_RegNumber = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_RegNumber",
	End;

	Object->STR_Reg_Name = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Name",
	End;

	Object->STR_Reg_Location = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Location",
	End;

	Object->STR_Reg_Voicenumber = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Voicenumber",
		MUIA_String_Accept, "0123456789.",
	End;

	Object->STR_Reg_Modem = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Modem",
		MUIA_String_Accept, "0123456789.+-",
	End;

	Object->STR_Reg_EMail = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_EMail",
	End;

	GR_grp_28 = GroupObject,
		MUIA_HelpNode, "GR_grp_28",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Your Registrarion Details",
		MUIA_Group_Columns, 2,
		Child, Label("Registration ID"),
		Child, Object->STR_RegNumber,
		Child, Label("Registration Name"),
		Child, Object->STR_Reg_Name,
		Child, Label("Registration Location"),
		Child, Object->STR_Reg_Location,
		Child, Label("Your Voicenumber"),
		Child, Object->STR_Reg_Voicenumber,
		Child, Label("1. Modemline or Telnet-Address"),
		Child, Object->STR_Reg_Modem,
		Child, Label("Your EMail Address"),
		Child, Object->STR_Reg_EMail,
	End;

	Object->BT_Reg_Save = SimpleButton("Save");

	GR_grp_29 = GroupObject,
		MUIA_HelpNode, "GR_grp_29",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Horiz, TRUE,
		Child, Label("Press Save, if you are finish"),
		Child, Object->BT_Reg_Save,
	End;

	GR_WindowRegistration = GroupObject,
		Child, Object->TX_label_7,
		Child, GR_grp_28,
		Child, GR_grp_29,
	End;

	Object->WindowRegistration = WindowObject,
		MUIA_Window_Title, "Registration",
		MUIA_Window_ID, MAKE_ID('3', 'W', 'I', 'N'),
		MUIA_Window_CloseGadget, FALSE,
		MUIA_Window_NoMenus, TRUE,
		WindowContents, GR_WindowRegistration,
	End;

	Object->TX_label_9 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_label_9,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->STR_CL_SystemPassword = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CL_SystemPassword",
	End;

	Object->STR_CL_Encrypt = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CL_Encrypt",
	End;

	GR_grp_36 = GroupObject,
		MUIA_HelpNode, "GR_grp_36",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("System Password"),
		Child, Object->STR_CL_SystemPassword,
		Child, Label("System Encryption"),
		Child, Object->STR_CL_Encrypt,
	End;

	Object->BT_CL_OK = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "OK",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_CL_OK",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	Object->BT_CL_CANCEL = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Cancel",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_CL_CANCEL",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;

	GR_grp_37 = GroupObject,
		MUIA_HelpNode, "GR_grp_37",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_SameSize, TRUE,
		Child, Object->BT_CL_OK,
		Child, HVSpace,
		Child, Object->BT_CL_CANCEL,
	End;

	Object->WindowLocked = WindowObject,
		MUIA_Window_Title, "Consule locked",
		MUIA_Window_ID, MAKE_ID('4', 'W', 'I', 'N'),
		WindowContents, GroupObject,
			Child, Object->TX_label_9,
			Child, GR_grp_36,
			Child, GR_grp_37,
		End,
	End;

	Object->STR_NC_PORTS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_PORTS",
		MUIA_String_Contents, "1",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 4,
	End;

	Object->STR_NC_FROMID = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_FROMID",
		MUIA_String_Accept, "0123456798",
		MUIA_String_MaxLen, 13,
	End;

	Object->STR_NC_TOID = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_TOID",
		MUIA_String_Accept, "0123456798",
		MUIA_String_MaxLen, 13,
	End;

	Object->CY_NC_NETWORKTYPE = CycleObject,
		MUIA_HelpNode, "CY_NC_NETWORKTYPE",
		MUIA_Cycle_Entries, Object->CY_NC_NETWORKTYPEContent,
	End;

	Object->RA_NC_STARTUPMODE = RadioObject,
		MUIA_Weight, 0,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_HelpNode, "RA_NC_STARTUPMODE",
		MUIA_Radio_Entries, Object->RA_NC_STARTUPMODEContent,
	End;

	GR_NC_STARTUPMODE = GroupObject,
		MUIA_HelpNode, "GR_NC_STARTUPMODE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->RA_NC_STARTUPMODE,
		Child, HSpace(0),
	End;

	Object->CH_NC_HIDE = CheckMark(FALSE);

	Object->CH_NC_INTERNALPORT = CheckMark(FALSE);

	Object->CH_NC_ANSIONLY = CheckMark(FALSE);

	Object->CH_NC_ADMINPORT = CheckMark(FALSE);

	Object->CH_NC_USEPASSWORD = CheckMark(FALSE);

	GR_NC_USEPASSWORD = GroupObject,
		MUIA_HelpNode, "GR_NC_USEPASSWORD",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NC_USEPASSWORD,
		Child, HVSpace,
	End;

	Object->STR_NC_PASSWORD = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_PASSWORD",
	End;

	Object->STR_NC_ENCRYPT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_ENCRYPT",
	End;

	Object->PA_NC_SERIAL_DEVICE = PopaslObject,
		MUIA_HelpNode, "PA_NC_SERIAL_DEVICE",
		MUIA_Weight, 4,
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_NC_SERIAL_DEVICE = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_SERIAL_DEVICE = PopButton(MUII_PopUp),
	End;

	Object->STR_NC_SERIAL_FROMUNIT = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_FROMUNIT",
		MUIA_String_Contents, "0",
		MUIA_String_Accept, "0123465789",
	End;

	Object->STR_NC_SERIAL_TOUNIT = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_TOUNIT",
		MUIA_String_Contents, "0",
		MUIA_String_Accept, "0123465789",
	End;

	GR_NC_SERIAL_DEVICE = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_DEVICE",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("Device"),
		Child, Object->PA_NC_SERIAL_DEVICE,
		Child, Label("From Unit"),
		Child, GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_FROMUNIT",
		MUIA_Group_Horiz, TRUE,
			Child, Object->STR_NC_SERIAL_FROMUNIT,
			Child, ImageObject,
				MUIA_Image_Spec, 13,
				MUIA_Weight, 0,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_Image_FreeVert, TRUE,
				MUIA_Image_FreeHoriz, TRUE,
				MUIA_FixHeight, 10,
				MUIA_FixWidth, 10,
			End,
			Child, ImageObject,
				MUIA_Image_Spec, 14,
				MUIA_Weight, 0,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_Image_FreeVert, TRUE,
				MUIA_Image_FreeHoriz, TRUE,
				MUIA_FixHeight, 10,
				MUIA_FixWidth, 10,
			End,
		End,
		Child, Label("To Unit"),
		Child, GroupObject,
			MUIA_HelpNode, "GR_NC_SERIAL_TOUNIT",
			MUIA_Group_Horiz, TRUE,
			Child, Object->STR_NC_SERIAL_TOUNIT,
			Child, ImageObject,
				MUIA_Image_Spec, 13,
				MUIA_Weight, 0,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_Image_FreeVert, TRUE,
				MUIA_Image_FreeHoriz, TRUE,
				MUIA_FixHeight, 10,
				MUIA_FixWidth, 10,
			End,
			Child, ImageObject,
				MUIA_Image_Spec, 14,
				MUIA_Weight, 0,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_Image_FreeVert, TRUE,
				MUIA_Image_FreeHoriz, TRUE,
				MUIA_FixHeight, 10,
				MUIA_FixWidth, 10,
			End,
		End,
		Child, Label("Shared"),
		Child, GroupObject,
			MUIA_HelpNode, "GR_NC_SERIAL_SHARED",
			MUIA_Group_Horiz, TRUE,
			Child, Object->CH_NC_SERIAL_SHARED = CheckMark(FALSE),
			Child, HVSpace,
		End,
	End;

	Object->STR_NC_SERIAL_IDLERATE = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_IDLERATE",
		MUIA_String_Contents, "115200",
		MUIA_String_Accept, "0123465789",
		MUIA_String_MaxLen, 13,
	End;

	GR_NC_SERIAL_BAUDRATE_IDLE = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_BAUDRATE_IDLE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_SERIAL_IDLERATE,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	Object->STR_NC_SERIAL_ONLINERATE = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_ONLINERATE",
		MUIA_String_Contents, "115200",
		MUIA_String_Accept, "0123465789",
		MUIA_String_MaxLen, 13,
	End;

	GR_NC_SERIAL_BAUDRATE_ONLINE = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_BAUDRATE_ONLINE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_SERIAL_ONLINERATE,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	Object->STR_NC_SERIAL_OFFLINERATE = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_OFFLINERATE",
		MUIA_String_Contents, "115200",
		MUIA_String_Accept, "0123465789",
		MUIA_String_MaxLen, 13,
	End;

	GR_NC_SERIAL_BAUDRATE_OFFLINE = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_BAUDRATE_OFFLINE",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_SERIAL_OFFLINERATE,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	Object->CY_NC_SERIAL_BITS = CycleObject,
		MUIA_HelpNode, "CY_NC_SERIAL_BITS",
		MUIA_Cycle_Entries, Object->CY_NC_SERIAL_BITSContent,
	End;

	Object->CY_NC_SERIAL_PARITY = CycleObject,
		MUIA_HelpNode, "CY_NC_SERIAL_PARITY",
		MUIA_Cycle_Entries, Object->CY_NC_SERIAL_PARITYContent,
	End;

	Object->CY_NC_SERIAL_PROTOCOLL = CycleObject,
		MUIA_HelpNode, "CY_NC_SERIAL_PROTOCOLL",
		MUIA_Cycle_Entries, Object->CY_NC_SERIAL_PROTOCOLLContent,
	End;

	Object->CY_NC_SERIAL_STOPBITS = CycleObject,
		MUIA_HelpNode, "CY_NC_SERIAL_STOPBITS",
		MUIA_Cycle_Entries, Object->CY_NC_SERIAL_STOPBITSContent,
	End;

	Object->CH_NC_SERIAL_DSR = ImageObject,
		MUIA_Background, MUII_HSHINEBACK,
		MUIA_InputMode, MUIV_InputMode_Toggle,
		MUIA_Image_Spec, MUII_CheckMark,
		MUIA_Image_FreeVert, TRUE,
		MUIA_Selected, TRUE,
		MUIA_ShowSelState, FALSE,
	End;

	GR_NC_SERIAL_FLAGS = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_FLAGS",
		MUIA_Group_Horiz, TRUE,
		Child, Object->CH_NC_SERIAL_8N1 = CheckMark(TRUE),
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("Check DSR"),
			Child, Object->CH_NC_SERIAL_DSR,
		End,
		Child, GroupObject,
			MUIA_Group_Columns, 2,
			Child, Label2("DTR-Logoff"),
			Child, Object->CH_NC_SERIAL_DTR = CheckMark(TRUE),
		End,
	End;

	Object->STR_NC_SERIAL_IOBUFFER = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_IOBUFFER",
		MUIA_String_Contents, "8196",
		MUIA_String_Accept, "0123465789",
	End;

	GR_NC_SERIAL_IOBUFFER = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_IOBUFFER",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_SERIAL_IOBUFFER,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	GR_NC_SERIAL_PARAMETER = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_PARAMETER",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("Baudrate Idle"),
		Child, GR_NC_SERIAL_BAUDRATE_IDLE,
		Child, Label("Baudrate Online"),
		Child, GR_NC_SERIAL_BAUDRATE_ONLINE,
		Child, Label("Baudrate Offline"),
		Child, GR_NC_SERIAL_BAUDRATE_OFFLINE,
		Child, Label("Bits"),
		Child, Object->CY_NC_SERIAL_BITS,
		Child, Label("Parity"),
		Child, Object->CY_NC_SERIAL_PARITY,
		Child, Label("Protocol"),
		Child, Object->CY_NC_SERIAL_PROTOCOLL,
		Child, Label("Stop Bits"),
		Child, Object->CY_NC_SERIAL_STOPBITS,
		Child, Label("8N1-Mode"),
		Child, GR_NC_SERIAL_FLAGS,
		Child, Label("IOBuffersize"),
		Child, GR_NC_SERIAL_IOBUFFER,
	End;

	Object->STR_NC_SERIAL_CONNECT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_CONNECT",
		MUIA_String_Contents, "CONNECT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_NOCARRIER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_NOCARRIER",
		MUIA_String_Contents, "NO CARRIER",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_NODIALTONE = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_NODIALTONE",
		MUIA_String_Contents, "NO DIALTONE",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_RING = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_RING",
		MUIA_String_Contents, "RING",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_BUSY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_BUSY",
		MUIA_String_Contents, "BUSY",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_OK = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_OK",
		MUIA_String_Contents, "OK",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_TIMEOUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_TIMEOUT",
		MUIA_String_Contents, "TIMEOUT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_ERROR = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_ERROR",
		MUIA_String_Contents, "ERROR",
		MUIA_String_MaxLen, 20,
	End;

	GR_NC_SERIAL_MODEM = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_MODEM",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("CONNECT"),
		Child, Object->STR_NC_SERIAL_CONNECT,
		Child, Label("NOCARRIER"),
		Child, Object->STR_NC_SERIAL_NOCARRIER,
		Child, Label("NODIALTONE"),
		Child, Object->STR_NC_SERIAL_NODIALTONE,
		Child, Label("RING"),
		Child, Object->STR_NC_SERIAL_RING,
		Child, Label("BUSY"),
		Child, Object->STR_NC_SERIAL_BUSY,
		Child, Label("OK"),
		Child, Object->STR_NC_SERIAL_OK,
		Child, Label("TIMEOUT"),
		Child, Object->STR_NC_SERIAL_TIMEOUT,
		Child, Label("ERROR"),
		Child, Object->STR_NC_SERIAL_ERROR,
	End;

	Object->STR_NC_SERIAL_INIT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_INIT",
		MUIA_String_Contents, "ATZ\r",
		MUIA_String_MaxLen, 10,
	End;

	Object->STR_NC_SERIAL_EXIT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_EXIT",
		MUIA_String_Contents, "~~~~~+++~~~~~",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_HANGUP = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_HANGUP",
		MUIA_String_Contents, "ATH0",
		MUIA_String_MaxLen, 10,
	End;

	Object->STR_NC_SERIAL_HANGDOWN = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_HANGDOWN",
		MUIA_String_Contents, "ATH1",
		MUIA_String_MaxLen, 10,
	End;

	GR_NC_SERIAL_CMD = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_CMD",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("Modem initialization command"),
		Child, Object->STR_NC_SERIAL_INIT,
		Child, Label("Modem exit command"),
		Child, Object->STR_NC_SERIAL_EXIT,
		Child, Label("Modem Hang up command"),
		Child, Object->STR_NC_SERIAL_HANGUP,
		Child, Label("Modem Hang down command"),
		Child, Object->STR_NC_SERIAL_HANGDOWN,
	End;

	Object->STR_NC_SERIAL_DIALPREFIX = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_DIALPREFIX",
		MUIA_String_Contents, "ATDT",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_DIALSUFFIX = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_DIALSUFFIX",
		MUIA_String_Contents, "\r",
		MUIA_String_MaxLen, 20,
	End;

	Object->CY_NC_SERIAL_DIALMODE = CycleObject,
		MUIA_HelpNode, "CY_NC_SERIAL_DIALMODE",
		MUIA_Cycle_Entries, Object->CY_NC_SERIAL_DIALMODEContent,
	End;

	Object->STR_NC_SERIAL_ANSWER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_ANSWER",
		MUIA_String_Contents, "ATA",
		MUIA_String_MaxLen, 20,
	End;

	Object->STR_NC_SERIAL_ANSWERTIMEOUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_ANSWERTIMEOUT",
		MUIA_String_Contents, "0",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 3,
	End;

	Object->STR_NC_SERIAL_RINGS = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_RINGS",
		MUIA_String_Contents, "0",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 3,
	End;

	Object->STR_NC_SERIAL_CONNECTTIMEOUT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_SERIAL_CONNECTTIMEOUT",
		MUIA_String_Contents, "30",
		MUIA_String_Accept, "012345679",
		MUIA_String_MaxLen, 3,
	End;

	GR_NC_SERIAL_DIAL = GroupObject,
		MUIA_HelpNode, "GR_NC_SERIAL_DIAL",
		MUIA_Background, MUII_BACKGROUND,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		Child, Label("Dial prefix"),
		Child, Object->STR_NC_SERIAL_DIALPREFIX,
		Child, Label("Dial suffix"),
		Child, Object->STR_NC_SERIAL_DIALSUFFIX,
		Child, Label("Dialmode"),
		Child, Object->CY_NC_SERIAL_DIALMODE,
		Child, Label("Answer command"),
		Child, Object->STR_NC_SERIAL_ANSWER,
		Child, Label("Answer timeout (secs)"),
		Child, Object->STR_NC_SERIAL_ANSWERTIMEOUT,
		Child, Label("Wait Rings"),
		Child, Object->STR_NC_SERIAL_RINGS,
		Child, Label("Connect Time (secs)"),
		Child, Object->STR_NC_SERIAL_CONNECTTIMEOUT,
	End;

	Object->GR_NC_SERIAL = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_NC_SERIAL,
		MUIA_HelpNode, "GR_NC_SERIAL",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Serial Network Config",
		Child, GR_NC_SERIAL_DEVICE,
		Child, GR_NC_SERIAL_PARAMETER,
		Child, GR_NC_SERIAL_MODEM,
		Child, GR_NC_SERIAL_CMD,
		Child, GR_NC_SERIAL_DIAL,
	End;

	Object->PA_NC_PAR_DEVICE = PopaslObject,
		MUIA_HelpNode, "PA_NC_PAR_DEVICE",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_NC_PAR_DEVICE = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_PAR_DEVICE = PopButton(MUII_PopFile),
	End;

	Object->CY_NC_PAR_CABLETYPE = CycleObject,
		MUIA_HelpNode, "CY_NC_PAR_CABLETYPE",
		MUIA_Cycle_Entries, Object->CY_NC_PAR_CABLETYPEContent,
	End;

	Object->STR_NC_PAR_FROMUNIT = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_PAR_FROMUNIT",
		MUIA_String_Accept, "0123465789",
	End;

	GR_NC_PAR_FROMUNIT = GroupObject,
		MUIA_HelpNode, "GR_NC_PAR_FROMUNIT",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_PAR_FROMUNIT,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	Object->STR_NC_PAR_TOUNIT = StringObject,
		MUIA_Weight, 1,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_PAR_TOUNIT",
		MUIA_String_Accept, "0123465789",
	End;

	GR_NC_PAR_TOUNIT = GroupObject,
		MUIA_HelpNode, "GR_NC_PAR_TOUNIT",
		MUIA_Group_Horiz, TRUE,
		Child, Object->STR_NC_PAR_TOUNIT,
		Child, ImageObject,
			MUIA_Image_Spec, 13,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
		Child, ImageObject,
			MUIA_Image_Spec, 14,
			MUIA_Weight, 0,
			MUIA_Frame, MUIV_Frame_ImageButton,
			MUIA_Image_FreeVert, TRUE,
			MUIA_Image_FreeHoriz, TRUE,
			MUIA_FixHeight, 10,
			MUIA_FixWidth, 10,
		End,
	End;

	Object->GR_NC_PAR = GroupObject,
		MUIA_HelpNode, "GR_NC_PAR",
		MUIA_ShowMe, FALSE,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Parallel Network Config",
		MUIA_Group_Columns, 2,
		Child, Label("Device"),
		Child, Object->PA_NC_PAR_DEVICE,
		Child, Label("Cable-Type"),
		Child, Object->CY_NC_PAR_CABLETYPE,
		Child, Label("From Unit"),
		Child, GR_NC_PAR_FROMUNIT,
		Child, Label("To Unit"),
		Child, GR_NC_PAR_TOUNIT,
	End;

	Object->LV_CC_TCPIP2_HOSTCC = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_CC_TCPIP2_HOSTCC = ListviewObject,
		MUIA_HelpNode, "LV_CC_TCPIP2_HOSTCC",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_CC_TCPIP2_HOSTCC,
	End;

	Object->STR_CC_TCPIP2_HOSTCC = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_TCPIP2_HOSTCC",
	End;

	GR_NC_TCPIP2_HOST = GroupObject,
		MUIA_HelpNode, "GR_NC_TCPIP2_HOST",
		Child, Object->LV_CC_TCPIP2_HOSTCC,
		Child, Object->STR_CC_TCPIP2_HOSTCC,
	End;

	Object->PO_NC_TCPIP_HOST = PopobjectObject,
		MUIA_HelpNode, "PO_NC_TCPIP_HOST",
		MUIA_Popstring_String, Object->STR_PO_NC_TCPIP_HOST = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, GR_NC_TCPIP2_HOST,
	End;

	Object->STR_NC_TCPIP_FROMPORT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_TCPIP_FROMPORT",
	End;

	Object->STR_NC_TCP_FROMIP = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_TCP_FROMIP",
	End;

	Object->STR_NC_TCP_TOIP = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_NC_TCP_TOIP",
	End;

	Object->STR_CC_TCPIP_TOPORT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_CC_TCPIP_TOPORT",
	End;

	Object->CY_NC_TCP_GETHOSTNAMEBY = CycleObject,
		MUIA_HelpNode, "CY_NC_TCP_GETHOSTNAMEBY",
		MUIA_Cycle_Entries, Object->CY_NC_TCP_GETHOSTNAMEBYContent,
	End;

	Object->GR_NC_TCP = GroupObject,
		MUIA_HelpNode, "GR_NC_TCP",
		MUIA_ShowMe, FALSE,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "TCP/IP Network Config",
		MUIA_Group_Columns, 2,
		Child, Label("Host"),
		Child, Object->PO_NC_TCPIP_HOST,
		Child, Label("From Port"),
		Child, Object->STR_NC_TCPIP_FROMPORT,
		Child, Label("From IP"),
		Child, Object->STR_NC_TCP_FROMIP,
		Child, Label("To IP"),
		Child, Object->STR_NC_TCP_TOIP,
		Child, Label("To Port"),
		Child, Object->STR_CC_TCPIP_TOPORT,
		Child, Label("Get Hostname by"),
		Child, Object->CY_NC_TCP_GETHOSTNAMEBY,
	End;

	Object->GR_NC_NETWORKCONFIG = GroupObject,
		MUIA_HelpNode, "GR_NC_NETWORKCONFIG",
		Child, Object->GR_NC_SERIAL,
		Child, Object->GR_NC_PAR,
		Child, Object->GR_NC_TCP,
	End;

	Object->PA_NC_SCREENMODE = PopaslObject,
		MUIA_HelpNode, "PA_NC_SCREENMODE",
		MUIA_Popasl_Type, 2,
		MUIA_Popstring_String, Object->STR_PA_NC_SCREENMODE = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_SCREENMODE = PopButton(MUII_PopUp),
	End;

	Object->CY_NC_COLORS = CycleObject,
		MUIA_HelpNode, "CY_NC_COLORS",
		MUIA_Cycle_Entries, Object->CY_NC_COLORSContent,
	End;

	Object->PA_NC_SCREENFONT = PopaslObject,
		MUIA_HelpNode, "PA_NC_SCREENFONT",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_NC_SCREENFONT = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_SCREENFONT = PopButton(MUII_PopUp),
	End;

	Object->PA_NC_TERMFONT = PopaslObject,
		MUIA_HelpNode, "PA_NC_TERMFONT",
		MUIA_Popasl_Type, 1,
		MUIA_Popstring_String, Object->STR_PA_NC_TERMFONT = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_TERMFONT = PopButton(MUII_PopUp),
	End;

	Object->PA_NC_STARTUPSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_NC_STARTUPSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_NC_STARTUPSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_STARTUPSCRIPT = PopButton(MUII_PopUp),
	End;

	Object->CY_NC_STARTUPSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_NC_STARTUPSCRIPT",
		MUIA_Cycle_Entries, Object->CY_NC_STARTUPSCRIPTContent,
	End;

	Object->PA_NC_CONNECTSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_NC_CONNECTSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_NC_CONNECTSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_CONNECTSCRIPT = PopButton(MUII_PopUp),
	End;

	Object->CY_NC_CONNECTSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_NC_CONNECTSCRIPT",
		MUIA_Cycle_Entries, Object->CY_NC_CONNECTSCRIPTContent,
	End;

	Object->PA_NC_LOGOFFSCRIPT = PopaslObject,
		MUIA_HelpNode, "PA_NC_LOGOFFSCRIPT",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, Object->STR_PA_NC_LOGOFFSCRIPT = String("", 80),
		MUIA_Popstring_Button, Object->PA_NC_LOGOFFSCRIPT = PopButton(MUII_PopFile),
	End;

	Object->CY_NC_LOGOFFSCRIPT = CycleObject,
		MUIA_HelpNode, "CY_NC_LOGOFFSCRIPT",
		MUIA_Cycle_Entries, Object->CY_NC_LOGOFFSCRIPTContent,
	End;

	Object->GR_NC_DEFAILT_CONFIG = RegisterObject,
		MUIA_Register_Titles, Object->STR_GR_NC_DEFAILT_CONFIG,
		MUIA_HelpNode, "GR_NC_DEFAILT_CONFIG",
		MUIA_Frame, MUIV_Frame_Group,
		Child, GroupObject,
			MUIA_HelpNode, "GR_NC_MISC",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 2,
			Child, Label("Ports"),
			Child, Object->STR_NC_PORTS,
			Child, Label("From IDNumber"),
			Child, Object->STR_NC_FROMID,
			Child, Label("To IDNumber"),
			Child, Object->STR_NC_TOID,
			Child, Label("Network Type"),
			Child, Object->CY_NC_NETWORKTYPE,
			Child, Label("Statupmode"),
			Child, GR_NC_STARTUPMODE,
			Child, Label("Flags"),
			Child, GR_NC_MISC_FLAGS = GroupObject,
				MUIA_HelpNode, "GR_NC_MISC_FLAGS",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Group_Horiz, TRUE,
				Child, Object->CH_NC_HIDE,
				Child, HVSpace,
				Child, Label("Internal Port"),
				Child, Object->CH_NC_INTERNALPORT,
				Child, HVSpace,
				Child, Label("ANSI only"),
				Child, Object->CH_NC_ANSIONLY,
				Child, HVSpace,
				Child, Label("Administation Port"),
				Child, Object->CH_NC_ADMINPORT,
			End,
			Child, Label("Use Port Password"),
			Child, GR_NC_USEPASSWORD,
			Child, Label("Port Passwort"),
			Child, Object->STR_NC_PASSWORD,
			Child, Label("Encryption"),
			Child, Object->STR_NC_ENCRYPT,
		End,
		Child, Object->GR_NC_NETWORKCONFIG,
		Child, GroupObject,
			MUIA_HelpNode, "GR_NC_MONITOR",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 2,
			Child, Label("Screenmode"),
			Child, Object->PA_NC_SCREENMODE,
			Child, Label("Use Workbench"),
			Child, GroupObject,
				MUIA_HelpNode, "GR_NC_USEWB",
				MUIA_Group_Horiz, TRUE,
				Child, Object->CH_NC_USEWB = CheckMark(FALSE),
				Child, HVSpace,
			End,
			Child, TextObject,
				MUIA_Text_PreParse, "\033r",
				MUIA_Text_Contents, "Colors",
				MUIA_InnerLeft, 0,
				MUIA_InnerRight, 0,
			End,
			Child, Object->CY_NC_COLORS,
			Child, Label("Screen Font"),
			Child, Object->PA_NC_SCREENFONT,
			Child, Label("Terminal Font"),
			Child, Object->PA_NC_TERMFONT,
		End,
		Child, GroupObject,
			MUIA_HelpNode, "GR_NC_SCRIPTS",
			MUIA_Background, MUII_BACKGROUND,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_Group_Columns, 3,
			Child, Label("Startup script"),
			Child, Object->PA_NC_STARTUPSCRIPT,
			Child, Object->CY_NC_STARTUPSCRIPT,
			Child, Label("Connect script"),
			Child, Object->PA_NC_CONNECTSCRIPT,
			Child, Object->CY_NC_CONNECTSCRIPT,
			Child, Label("Logoff script"),
			Child, Object->PA_NC_LOGOFFSCRIPT,
			Child, Object->CY_NC_LOGOFFSCRIPT,
		End,
	End;

	Object->BT_NC_OTHERPORT = TextObject,
		ButtonFrame,
		MUIA_Weight, 0,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Load Config from other Port",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_NC_OTHERPORT",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
	End;


	Object->BT_NC_ACCEPT = SimpleButton("Accept");

	Object->BT_NC_CANCEL = SimpleButton("Cancel");

	Object->STR_USER_IDNUMBER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_IDNUMBER",
	End;

	Object->TX_USER_IPNUMBER = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_USER_IPNUMBER,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->TX_USER_STATUS = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_USER_STATUS,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->STR_USER_USERNAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_USERNAME",
		MUIA_String_MaxLen, 21,
	End;

	Object->STR_USER_REALNAME = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_REALNAME",
		MUIA_String_MaxLen, 41,
	End;

	Object->STR_USER_STREET = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_STREET",
		MUIA_String_MaxLen, 41,
	End;

	Object->STR_USER_ZIPCODE = StringObject,
		MUIA_Weight, 50,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_ZIPCODE",
		MUIA_String_MaxLen, 11,
	End;

	Object->STR_USER_CITY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_CITY",
		MUIA_String_MaxLen, 31,
	End;

	Object->STR_USER_COUNTRY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_COUNTRY",
		MUIA_String_MaxLen, 4,
	End;

	Object->STR_USER_AREACODE = StringObject,
		MUIA_Weight, 33,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_AREACODE",
		MUIA_String_MaxLen, 8,
	End;

	Object->STR_USER_PHONENUMBER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_PHONENUMBER",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 25,
	End;

	Object->STR_USER_MODEMNUMBER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_MODEMNUMBER",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 25,
	End;

	Object->STR_USER_FAXNUMBER = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_FAXNUMBER",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 25,
	End;

	Object->STR_USER_BIRTHDAY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_BIRTHDAY",
	End;

	Object->TX_USER_FIRSTCALL = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_USER_FIRSTCALL,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->TX_USER_LASTCALL = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_USER_LASTCALL,
		MUIA_Text_SetMin, TRUE,
	End;

	Object->LV_USER_COMPUTER = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_USER_COMPUTER = ListviewObject,
		MUIA_HelpNode, "LV_USER_COMPUTER",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_USER_COMPUTER,
	End;

	Object->PO_USER_COMPUTER = PopobjectObject,
		MUIA_HelpNode, "PO_USER_COMPUTER",
		MUIA_Popstring_String, Object->STR_PO_USER_COMPUTER = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_USER_COMPUTER,
	End;

	Object->LV_USER_ACCESS = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_USER_ACCESS = ListviewObject,
		MUIA_HelpNode, "LV_USER_ACCESS",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_USER_ACCESS,
	End;

	Object->PO_USER_ACCESS = PopobjectObject,
		MUIA_HelpNode, "PO_USER_ACCESS",
		MUIA_Popstring_String, Object->STR_PO_USER_ACCESS = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_USER_ACCESS,
	End;

	Object->LV_USER_ACCESSGROUP = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_USER_ACCESSGROUP = ListviewObject,
		MUIA_HelpNode, "LV_USER_ACCESSGROUP",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_USER_ACCESSGROUP,
	End;

	Object->PO_USER_ACCESSGROUP = PopobjectObject,
		MUIA_HelpNode, "PO_USER_ACCESSGROUP",
		MUIA_Popstring_String, Object->STR_PO_USER_ACCESSGROUP = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_USER_ACCESSGROUP,
	End;

	Object->STR_USER_EXPIREDAY = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_EXPIREDAY",
	End;

	Object->STR_USER_EXPIREDAYS = StringObject,
		MUIA_Weight, 50,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_EXPIREDAYS",
		MUIA_String_MaxLen, 20,
	End;

	Object->CY_USER_CHARSET = CycleObject,
		MUIA_HelpNode, "CY_USER_CHARSET",
		MUIA_Cycle_Entries, Object->CY_USER_CHARSETContent,
	End;

	Object->LV_USER_TEXTTYPE = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
	End;

	Object->LV_USER_TEXTTYPE = ListviewObject,
		MUIA_HelpNode, "LV_USER_TEXTTYPE",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_USER_TEXTTYPE,
	End;

	Object->PO_USER_TEXTTYPE = PopobjectObject,
		MUIA_HelpNode, "PO_USER_TEXTTYPE",
		MUIA_Popstring_String, Object->STR_PO_USER_TEXTTYPE = String("", 80),
		MUIA_Popstring_Button, PopButton(MUII_PopUp),
		MUIA_Popobject_Object, Object->LV_USER_TEXTTYPE,
	End;

	Object->CY_USER_MOREMODE = CycleObject,
		MUIA_HelpNode, "CY_USER_MOREMODE",
		MUIA_Cycle_Entries, Object->CY_USER_MOREMODEContent,
	End;

	Object->CY_USER_TIMEFORMAT = CycleObject,
		MUIA_HelpNode, "CY_USER_TIMEFORMAT",
		MUIA_Cycle_Entries, Object->CY_USER_TIMEFORMATContent,
	End;

	Object->CY_USER_DATEFORMAT = CycleObject,
		MUIA_HelpNode, "CY_USER_DATEFORMAT",
		MUIA_Cycle_Entries, Object->CY_USER_DATEFORMATContent,
	End;

	Object->CY_USER_LINEFEEDS = CycleObject,
		MUIA_HelpNode, "CY_USER_LINEFEEDS",
		MUIA_Cycle_Entries, Object->CY_USER_LINEFEEDSContent,
	End;

	Object->CY_USER_TERMTABS = CycleObject,
		MUIA_HelpNode, "CY_USER_TERMTABS",
		MUIA_Cycle_Entries, Object->CY_USER_TERMTABSContent,
	End;

	Object->CY_USER_TERMANSI = CycleObject,
		MUIA_HelpNode, "CY_USER_TERMANSI",
		MUIA_Cycle_Entries, Object->CY_USER_TERMANSIContent,
	End;

	Object->CY_USER_TERMCOLORS = CycleObject,
		MUIA_HelpNode, "CY_USER_TERMCOLORS",
		MUIA_Cycle_Entries, Object->CY_USER_TERMCOLORSContent,
	End;

	Object->STR_USER_TERMWIDTH = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_TERMWIDTH",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 4,
	End;

	Object->STR_USER_TERMLENGTH = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_USER_TERMLENGTH",
		MUIA_String_Accept, "0123456789",
		MUIA_String_MaxLen, 4,
	End;

	Object->App = ApplicationObject,
		MUIA_Application_Author, "Thorsten Gehler",
		MUIA_Application_Base, "NOVIA_GUI",
		MUIA_Application_Title, "Novia_gui",
		MUIA_Application_Version, "$VER : 0.00.08 (22.05.98)",
		MUIA_Application_Copyright, "© 1996-1999 Thorsten Gehler",
		MUIA_Application_Description, "Novia MUI-GUI Interface",
		MUIA_HelpFile, "novia.guide",
		SubWindow, Object->WindowClientList,
		SubWindow, Object->WindowClientConfig,
		SubWindow, Object->WindowSystemService,
		SubWindow, Object->WindowRegistration,
		SubWindow, Object->WindowLocked,
		SubWindow, Object->WindowNewClient = WindowObject,
			MUIA_Window_Title, "New Port Configuration",
			MUIA_Window_ID, MAKE_ID('5', 'W', 'I', 'N'),
			WindowContents, GroupObject,
				MUIA_FramePhantomHoriz, TRUE,
				Child, GroupObject,
				MUIA_HelpNode, "GR_grp_103",
				MUIA_Background, MUII_BACKGROUND,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_FrameTitle, "Default Configuration",
				Child, Object->GR_NC_DEFAILT_CONFIG,
				Child, GroupObject,
					MUIA_HelpNode, "GR_grp_91",
					MUIA_FramePhantomHoriz, TRUE,
					MUIA_Background, MUII_BACKGROUND,
					MUIA_Frame, MUIV_Frame_Group,
					MUIA_Group_Columns, 2,
					Child, Object->BT_NC_OTHERPORT,
					Child, HVSpace,
				End,
			End,
				Child, GroupObject,
					MUIA_HelpNode, "GR_NC_ACTION",
					MUIA_Group_Horiz, TRUE,
					Child, Object->BT_NC_ACCEPT,
					Child, HVSpace,
					Child, Object->BT_NC_CANCEL,
				End,
			End,
		End,
		SubWindow, Object->WindowUserEditor = WindowObject,
			MUIA_Window_Title, "UserEditor",
			MUIA_Window_ID, MAKE_ID('6', 'W', 'I', 'N'),
			WindowContents, GroupObject,
				MUIA_Frame, MUIV_Frame_Group,
				MUIA_FrameTitle, "Personal",
				Child, GroupObject,
					MUIA_HelpNode, "GR_USER",
					MUIA_Group_Columns, 2,
					Child, Label("IDNumber"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_STATUS",
						MUIA_Group_Horiz, TRUE,
						Child, Object->STR_USER_IDNUMBER,
						Child, Label("IP Number"),
						Child, Object->TX_USER_IPNUMBER,
						Child, Label("Status"),
						Child, Object->TX_USER_STATUS,
					End,
					Child, Label("Username"),
					Child, Object->STR_USER_USERNAME,
					Child, Label("Realname"),
					Child, Object->STR_USER_REALNAME,
					Child, Label("Streat"),
					Child, Object->STR_USER_STREET,
					Child, Label("ZIP Code"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_ADDRESS",
						MUIA_Group_Horiz, TRUE,
						Child, Object->STR_USER_ZIPCODE,
						Child, Label("City"),
						Child, Object->STR_USER_CITY,
						Child, Label("Country"),
						Child, Object->STR_USER_COUNTRY,
					End,
					Child, Label("Area Code"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_NUMBERS",
						MUIA_Group_Horiz, TRUE,
						Child, Object->STR_USER_AREACODE,
						Child, Label("Phonenumber #"),
						Child, Object->STR_USER_PHONENUMBER,
						Child, Label("Modemnumber #"),
						Child, Object->STR_USER_MODEMNUMBER,
						Child, Label("FAX number #"),
						Child, Object->STR_USER_FAXNUMBER,
					End,
					Child, Label("Birthday"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_DATES",
						MUIA_Group_Horiz, TRUE,
						Child, Object->STR_USER_BIRTHDAY,
						Child, Label("Firstcall"),
						Child, Object->TX_USER_FIRSTCALL,
						Child, Label("Lastcall"),
						Child, Object->TX_USER_LASTCALL,
						Child, HSpace(0),
					End,
					Child, Label("Computertype"),
					Child, Object->PO_USER_COMPUTER,
					Child, Label("Access"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_ACCESS",
						MUIA_Group_Horiz, TRUE,
						Child, Object->PO_USER_ACCESS,
						Child, Label("Accessgroup"),
						Child, Object->PO_USER_ACCESSGROUP,
					End,
					Child, Label("Expire on"),
					Child, GroupObject,
						MUIA_HelpNode, "GR_USER_EXPIRE",
						MUIA_Group_Horiz, TRUE,
						Child, Object->STR_USER_EXPIREDAY,
						Child, Label("Expire days"),
						Child, Object->STR_USER_EXPIREDAYS,
						Child, HVSpace,
					End,
				End,
				Child, GroupObject,
					MUIA_HelpNode, "GR_USER_TERMINALSETTINGS",
					MUIA_Frame, MUIV_Frame_Group,
					MUIA_FrameTitle, "Terminal Settings",
					MUIA_Group_Columns, 4,
					Child, Label("Charset"),
					Child, Object->CY_USER_CHARSET,
					Child, Label("Text type"),
					Child, Object->PO_USER_TEXTTYPE,
					Child, Label("More mode"),
					Child, Object->CY_USER_MOREMODE,
					Child, Label("Timeformat"),
					Child, Object->CY_USER_TIMEFORMAT,
					Child, Label("Dateformat"),
					Child, Object->CY_USER_DATEFORMAT,
					Child, Label("Linefeeds"),
					Child, Object->CY_USER_LINEFEEDS,
					Child, Label("Termtabs"),
					Child, Object->CY_USER_TERMTABS,
					Child, Label("Term ANSI"),
					Child, Object->CY_USER_TERMANSI,
					Child, Label("Term Colors"),
					Child, Object->CY_USER_TERMCOLORS,
					Child, HVSpace,
					Child, HVSpace,
					Child, Label("Term Width"),
					Child, Object->STR_USER_TERMWIDTH,
					Child, Label("Term Length"),
					Child, Object->STR_USER_TERMLENGTH,
				End,
			End,
		End,
		SubWindow, Object->ABOUT = AboutmuiObject,
			MUIA_Window_RefWindow, Object->WindowClientList = WindowObject,
				MUIA_Window_Title, "ClientList",
				MUIA_HelpNode, "WindowClientList",
				MUIA_Window_Menustrip, Object->MN_label_1 = MenustripObject,
					MUIA_Family_Child, Object->MNlabel1Project = MenuitemObject,
						MUIA_Menuitem_Title, "Project",
						MUIA_Family_Child, Object->MNlabel1About = MenuitemObject,
							MUIA_Menuitem_Title, "About...",
							MUIA_Menuitem_Shortcut, "?",
						End,
						MUIA_Family_Child, Object->MNlabel1AboutMUI = MenuitemObject,
							MUIA_Menuitem_Title, "About MUI...",
						End,
						MUIA_Family_Child, MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0),
						MUIA_Family_Child, Object->MNlabel1Lockconsule = MenuitemObject,
							MUIA_Menuitem_Title, "Lock consule",
							MUIA_Menuitem_Shortcut, "L",
						End,
						MUIA_Family_Child, MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0),
						MUIA_Family_Child, Object->MNlabel1RestartServer = MenuitemObject,MUIA_Menuitem_Title, "Restart Server",	End,
						MUIA_Family_Child, Object->MNlabel1ResetServer = MenuitemObject,	MUIA_Menuitem_Title, "Reset Server",	End,
						MUIA_Family_Child, MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0),
						MUIA_Family_Child, Object->MNlabel1Quit			= MenuitemObject,	MUIA_Menuitem_Title, "Quit",				End,
					End,
					MUIA_Family_Child, Object->MNlabel1SystemService = MenuitemObject,
						MUIA_Menuitem_Title, "System Service",
						MUIA_Family_Child, Object->MNlabel1LocalLogin	= MenuitemObject,	MUIA_Menuitem_Title, "Local Login",		End,
						MUIA_Family_Child, Object->MNlabel1NoNewUsers	= MenuitemObject,	MUIA_Menuitem_Title, "No New Users",	End,
						MUIA_Family_Child, Object->MNlabel1busyallports = MenuitemObject,	MUIA_Menuitem_Title, "busy all ports",	End,
					End,
					MUIA_Family_Child, Object->MNlabel1Areas = MenuitemObject,
						MUIA_Menuitem_Title, "Areas",
						MUIA_Family_Child, Object->MNlabel1MessageBase = MenuitemObject,
							MUIA_Menuitem_Title, "MessageBase",
							MUIA_Family_Child, Object->MNlabel1Disable_Base = MenuitemObject, MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_Base	= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1Filebase = MenuitemObject,
							MUIA_Menuitem_Title, "Filebase",
							MUIA_Family_Child, Object->MNlabel1Disable_UDBase= MenuitemObject,MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_UDBase= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1ChatChannels = MenuitemObject,
							MUIA_Menuitem_Title, "ChatChannels",
							MUIA_Family_Child, Object->MNlabel1Disable_CC = MenuitemObject,	MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, MNlabel1Config_CC				= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1News = MenuitemObject,
							MUIA_Menuitem_Title, "News",
							MUIA_Family_Child, Object->MNlabel1Disable_News = MenuitemObject,	MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_News	= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1PFiles = MenuitemObject,
							MUIA_Menuitem_Title, "PFiles",
							MUIA_Family_Child, Object->MNlabel1Disable_PFiles = MenuitemObject,	MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_PFiles= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1GFiles = MenuitemObject,
							MUIA_Menuitem_Title, "GFiles",
							MUIA_Family_Child, Object->MNlabel1Disable_GFiles = MenuitemObject,	MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_GFiles= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
						MUIA_Family_Child, Object->MNlabel1Votes = MenuitemObject,
							MUIA_Menuitem_Title, "Votes",
							MUIA_Family_Child, Object->MNlabel1Disable_Votes = MenuitemObject,	MUIA_Menuitem_Title, "Disable",	MUIA_Menuitem_Checkit, TRUE,	End,
							MUIA_Family_Child, Object->MNlabel1Config_Config= MenuitemObject,	MUIA_Menuitem_Title, "Config",			End,
						End,
					End,
				End,
				MUIA_Window_ID, MAKE_ID('0', 'W', 'I', 'N'),
				WindowContents, Object->GR_WindowClientList = GroupObject,
					Child, GroupObject,
						MUIA_HelpNode, "GR_maintance",
						MUIA_Group_Horiz, TRUE,
						Child, Label("Maintance"),
						Child, Object->BT_System_Service = TextObject,
							ButtonFrame,
							MUIA_Weight, 1,
							MUIA_Background, MUII_ButtonBack,
							MUIA_ControlChar, 'S',
							MUIA_Text_Contents, "System Service",
							MUIA_Text_PreParse, "\033c",
							MUIA_Text_HiChar, 'S',
							MUIA_HelpNode, "BT_System_Service",
							MUIA_InputMode, MUIV_InputMode_RelVerify,
						End,
					End,
					Child, RectangleObject,
						MUIA_Rectangle_HBar, TRUE,
						MUIA_FixHeight, 8,
					End,
					Child, Object->LV_Client_List,
					Child, GroupObject,
						MUIA_HelpNode, "GR_Action_Client",
						MUIA_Group_Horiz, TRUE,
						Child, GroupObject,
							MUIA_HelpNode, "GR_Client",
							MUIA_Group_Horiz, TRUE,
							Child, Label("Port"),
							Child, Object->CL_Status = TextObject,
								ButtonFrame,
								MUIA_Background, MUII_ButtonBack,
								MUIA_ControlChar, 'C',
								MUIA_Text_Contents, "Status",
								MUIA_Text_PreParse, "\033c",
								MUIA_Text_HiChar, 'C',
								MUIA_HelpNode, "CL_Status",
								MUIA_InputMode, MUIV_InputMode_RelVerify,
							End,
							Child, Object->BT_Config = SimpleButton("_Config"),
							Child, Object->CL_PORT_ACTION = CycleObject,
								MUIA_HelpNode, "CL_PORT_ACTION",
								MUIA_Cycle_Entries, Object->CL_PORT_ACTIONContent,
							End,
						End,
						Child, GroupObject,
							MUIA_HelpNode, "GR_Action",
							MUIA_Group_Horiz, TRUE,
							Child, Label("User"),
							Child, Object->BT_Userinfo = TextObject,
								ButtonFrame,
								MUIA_Weight, 0,
								MUIA_Background, MUII_ButtonBack,
								MUIA_ControlChar, 'U',
								MUIA_Text_Contents, "Userinfo",
								MUIA_Text_PreParse, "\033c",
								MUIA_Text_HiChar, 'U',
								MUIA_HelpNode, "BT_Userinfo",
								MUIA_InputMode, MUIV_InputMode_RelVerify,
							End,
							Child, Object->BT_Portinfo = TextObject,
								ButtonFrame,
								MUIA_Weight, 0,
								MUIA_Background, MUII_ButtonBack,
								MUIA_ControlChar, 'i',
								MUIA_Text_Contents, "Portinfo",
								MUIA_Text_PreParse, "\033c",
								MUIA_Text_HiChar, 'i',
								MUIA_HelpNode, "BT_Portinfo",
								MUIA_InputMode, MUIV_InputMode_RelVerify,
							End,
							Child, Object->BT_Mailbox = TextObject,
								ButtonFrame,
								MUIA_Weight, 0,
								MUIA_Background, MUII_ButtonBack,
								MUIA_ControlChar, 'M',
								MUIA_Text_Contents, "Mailbox",
								MUIA_Text_PreParse, "\033c",
								MUIA_Text_HiChar, 'M',
								MUIA_HelpNode, "BT_Mailbox",
								MUIA_InputMode, MUIV_InputMode_RelVerify,
							End,
						End,
					End,
				End,
			End,
			MUIA_Aboutmui_Application, Object->ABOUT,
		End,
	End;


	if (!Object->App)
	{
		FreeVec(Object);
		return(NULL);
	}

	DoMethod(Object->App,
		MUIM_Notify, MUIA_Application_Iconified, TRUE,
		Object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(Object->App,
		MUIM_Notify, MUIA_Application_Iconified, TRUE,
		Object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(Object->BT_System_Service,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->WindowSystemService,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
		);

	DoMethod(Object->WindowClientList,
		MUIM_Window_SetCycleChain, Object->GR_WindowClientList,
		Object->BT_System_Service,
		Object->LV_Client_List,
		Object->CL_Status,
		Object->BT_Config,
		Object->CL_PORT_ACTION,
		Object->BT_Userinfo,
		Object->BT_Portinfo,
		Object->BT_Mailbox,
		0
		);

	DoMethod(Object->CH_CC_Workbench,
		MUIM_Notify, MUIA_Selected, TRUE,
		Object->PA_Screenmode,
		3,
		MUIM_Set, MUIA_Disabled, TRUE
		);

	DoMethod(Object->CH_CC_Workbench,
		MUIM_Notify, MUIA_Selected, FALSE,
		Object->PA_Screenmode,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(Object->WindowClientConfig,
		MUIM_Window_SetCycleChain, Object->GR_WindowClientConfig,
		Object->GR_grp_10,
		Object->STR_CC_IDNUMBER,
		Object->STR_CC_NAME,
		Object->CY_CC_NETWORKTYPE,
		Object->RA_CC_STARTUPMODE,
		Object->CH_CC_HIDE,
		Object->CH_CC_INTERNALPORT,
		Object->CH_CC_ANSIONLY,
		Object->GR_NetworkConfig,
		Object->GR_SerialConfig,
		Object->PA_Device_Name,
		Object->STR_SerConfig_Unitno,
		Object->STR_ClientConfig_Baudrateidle,
		Object->STR_Baudrateonline,
		Object->STR_CC_Baudrateoffline,
		Object->CY_CC_Bits,
		Object->CY_CC_Parity,
		Object->CY_CC_Protocoll,
		Object->CY_CC_StopBits,
		Object->CH_CC_DSR,
		Object->CH_CC_DTR,
		Object->STR_SerConfig_IOBuffer,
		Object->STR_Modem_CONNECT,
		Object->STR_Modem_NOCARRIER,
		Object->STR_Modem_NODIALTONE,
		Object->STR_Modem_RING,
		Object->STR_Modem_BUSY,
		Object->STR_Modem_OK,
		Object->STR_Modem_TIMEOUT,
		Object->STR_Modem_ERROR,
		Object->STR_Commands_init,
		Object->STR_Commands_exit,
		Object->STR_Commands_hangup,
		Object->STR_Dial_prefix,
		Object->STR_Dial_suffix,
		Object->CY_Dial_Dialmode,
		Object->STR_Answer_anscmd,
		Object->GR_Parallel,
		Object->PA_DeviceParallel,
		Object->CY_CC_par_cabletype,
		Object->GR_TCPIP,
		Object->PO_CC_TCPIP_HOST,
		Object->STR_CC_TCPIP_PORT,
		Object->PA_TerminalFont,
		Object->CH_CC_Workbench,
		Object->CH_CC_openmonitor,
		Object->CY_Colors,
		Object->PA_ScreenFont,
		Object->PA_Screenmode,
		Object->PA_CC_STARTUPSCRIPT,
		Object->CY_CC_STARTUPSCRIPT,
		Object->PA_CC_CONNECTSCRIPT,
		Object->CY_CC_CONNECTSCRIPT,
		Object->PA_CC_LOGOFFSCRIPT,
		Object->CY_CC_LOGOFFSCRIPT,
		Object->BT_CC_Save,
		Object->BT_CC_Use,
		Object->BT_CC_Cancel,
		0
		);

	DoMethod(Object->CH_Main,
		MUIM_Notify, MUIA_Selected, TRUE,
		Object->PA_Maintance_Screenmode,
		3,
		MUIM_Set, MUIA_Disabled, TRUE
		);

	DoMethod(Object->CH_Main,
		MUIM_Notify, MUIA_Selected, FALSE,
		Object->PA_Maintance_Screenmode,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(Object->BT_label_65,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_PARENT
		);

	DoMethod(Object->LV_SS_BASEVIEW,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_SELECT
		);

	DoMethod(Object->BT_SS_BASE_OPEN,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_OPEN
		);

	DoMethod(Object->BT_SS_BASE_EDIT,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_EDIT
		);

	DoMethod(Object->BT_SS_BASE_NEW,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_NEW
		);

	DoMethod(Object->BT_SS_BASE_REMOVE,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_BASE_REMOVE
		);

	DoMethod(Object->LV_SS_TCP_MAILBOX_HOSTS,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_CallHook, &DisplayListMailserverUpdateHook
		);

	DoMethod(Object->STR_SS_TCP_MAILBOX_NAME,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_CHANGEHOST
		);

	DoMethod(Object->STR_SS_TCP_MAILBOX_DOMAIN,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		Object->TX_SS_TCP_MAILBOX_INVALID_NAME,
		3,
		MUIM_Set, MUIA_Text_Contents, MUIV_TriggerValue
		);

	DoMethod(Object->LV_SS_TCP_MAILBOX_USERS_NAME,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_USERSELECT_DC
		);

	DoMethod(Object->LV_SS_TCP_MAILBOX_USERS_NAME,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		Object->GR_SS_TCP_MAILBOX_USERS_NAME,
		3,
		MUIM_Set, MUIA_ShowMe, FALSE
		);

	DoMethod(Object->STR_SS_TCP_MAILBOX_USERS_NAME,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_TCP_MAILBOX_USERSELECT_CHANGE
		);

	DoMethod(Object->LV_SS_TCP_POP_HOSTS,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_CallHook, &DisplayListPOP3UpdateHook
		);

	DoMethod(Object->LV_REMINDER,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_CallHook, &DisplayListEventUpdateHook
		);

	DoMethod(Object->STR_SS_TCP_POP_NAME,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_SS_TCP_POP3_CHANGEHOST
		);

	DoMethod(Object->STR_SS_TCP_POP_DOMAIN,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		Object->TX_SS_TCP_POP_INVALID_NAME,
		3,
		MUIM_Set, MUIA_Text_Contents, MUIV_TriggerValue
		);

	DoMethod(Object->WindowSystemService,
		MUIM_Window_SetCycleChain, Object->GR_WindowSystemService,
		Object->STR_SystemName,
		Object->STR_RegLocation,
		Object->STR_MainPortNumber,
		Object->STR_UUCPDomain,
		Object->PA_SS_MAINPATH,
		Object->PA_SS_USERDIR,
		Object->PA_SS_SYSDATA,
		Object->PA_SS_SYSTEXT,
		Object->PA_SS_NEWUSER,
		Object->STR_SysopAccountNo,
		Object->STR_DefaultAreaCode,
		Object->STR_DefaultLocationNo,
		Object->STR_DefaultCountryNo,
		Object->PA_Maintance_Screenmode,
		Object->CH_Main,
		Object->CH_SS_loadgui,
		Object->CY_SS_Options_Colors,
		Object->PA_OptionsFont,
		Object->LV_SS_BASEVIEW,
		Object->BT_SS_BASE_OPEN,
		Object->BT_SS_BASE_EDIT,
		Object->BT_SS_BASE_NEW,
		Object->BT_SS_BASE_REMOVE,
		Object->LV_SS_NEWS,
		Object->STR_SS_NEWS,
		Object->BT_NEWS_NEW,
		Object->BT_NEWS_ADD,
		Object->BT_NEWS_REMOVE,
		Object->STR_NEWS_EDIT_NAME,
		Object->PO_NEWS_EDIT_ACCOUTLEVEL,
		Object->CY_NEWS_EDIT_STATUS,
		Object->LV_SS_PFILES,
		Object->STR_SS_PFILES,
		Object->BT_PFILES_NEW,
		Object->BT_PFILES_ADD,
		Object->BT_PFILES_REMOVE,
		Object->STR_PFILES_EDIT_NAME,
		Object->PO_PFILES_EDIT_ACCOUTLEVEL,
		Object->CY_PFILES_EDIT_STATUS,
		Object->LV_SS_NRC_LIST,
		Object->STR_NRC_ROOMID,
		Object->STR_NRC_ROOMNAME,
		Object->STR_NRC_PASSWORD,
		Object->STR_NRC_ENCRYPT,
		Object->STR_NRC_MAXIRCUSERS,
		Object->STR_NRC_MAXLOCALUSERS,
		Object->CH_NRC_FLAG_INTERNAL,
		Object->CH_NRC_FLAG_MEMBERSONLY,
		Object->CH_NRC_FLAG_USEPW,
		Object->CH_NRC_FLAG_BROADCAST,
		Object->CH_NRC_FLAG_SYSMSG,
		Object->CH_NRC_FLAG_ANONYMOUS,
		Object->CH_NRC_FLAG_IRCCHANNEL,
		Object->CH_NRC_FLAG_REALNAMES,
		Object->STR_NRC_IRC_IP,
		Object->STR_NRC_IRC_PORT,
		Object->CH_NRC_OPENIRCATSTART,
		Object->STR_NRC_IRC_PASSWORD,
		Object->STR_NRC_IRC_CHANNELNAME,
		Object->LV_NRC_MEMBERS,
		Object->BT_NRC_MEMBERS_ADD,
		Object->BT_NRC_MEMBERS_REMOVE,
		Object->LV_NRC_GROUPS,
		Object->BT_NRC_GROUPS_ADD,
		Object->BT_NRC_GROUPS_REMOVE,
		Object->STR_SS_TCP_REMOTENODES,
		Object->STR_SS_TCP_LOCALHOST,
		Object->CH_SS_TCP_REMOTEADMIN,
		Object->STR_SS_TCP_REMOTEIP,
		Object->STR_SS_TCP_Port,
		Object->CH_SS_TCP_MISC_SUMMERTIME,
		Object->LV_SS_TCP_MAILBOX_HOSTS,
		Object->BT_SS_TCP_MAILBOX_HOSTS_NEW,
		Object->BT_SS_TCP_MAILBOX_HOSTS_REMOVE,
		Object->GR_SS_TCP_MAILBOX_PARAMETERS,
		Object->STR_SS_TCP_MAILBOX_NAME,
		Object->STR_SS_TCP_MAILBOX_DOMAIN,
		Object->STR_SS_TCP_MAILBOX_POPHOST,
		Object->STR_SS_TCP_MAILBOX_POPPORT,
		Object->STR_SS_TCP_MAILBOX_SMTPHOST,
		Object->STR_SS_TCP_MAILBOX_SMTPPORT,
		Object->CH_SS_TCP_MAILBOX_8BIT,
		Object->CH_SS_TCP_MAILBOX_APOP,
		Object->CH_SS_TCP_MAILBOX_EXTSERVER,
		Object->STR_SS_TCP_MAILBOX_INVALID_SUBJECT,
		Object->PA_SS_TCP_MAILBOX_INVALID_TEXT,
		Object->STR_SS_TCP_MAILBOX_INVALID_NAME,
		Object->PA_SS_TCP_MAILBOX_INBOUND,
		Object->PA_SS_TCP_MAILBOX_OUTBOUND,
		Object->GR_SS_TCP_MAILBOX_USERS,
		Object->LV_SS_TCP_MAILBOX_USERS,
		Object->BT_SS_TCP_MAILBOX_USERS_NEW,
		Object->BT_SS_TCP_MAILBOX_USERS_REMOVE,
		Object->GR_SS_TCP_MAILBOX_USERS_PARAMETERS,
		Object->PO_SS_TCP_MAILBOX_USERS_NAME,
		Object->CY_SS_TCP_USERS_TYPE,
		Object->STR_SS_TCP_MAILBOX_USERS_POPID,
		Object->CH_SS_TCP_MAILBOX_USERS_POPID_FROMBBS,
		Object->STR_SS_TCP_MAILBOX_USERS_PW,
		Object->CH_SS_TCP_MAILBOX_USERS_PW_FROMBBS,
		Object->STR_SS_TCP_MAILBOX_USERS_HOLDMAILS,
		Object->STR_SS_TCP_MAILBOX_USERS_MAXMAILS,
		Object->STR_SS_TCP_MAILBOX_USERS_SPACE,
		Object->LV_SS_TCP_POP_HOSTS,
		Object->BT_SS_TCP_POP_HOSTS_NEW,
		Object->BT_SS_TCP_POP_HOSTS_REMOVE,
		Object->GR_SS_TCP_POP_PARA,
		Object->STR_SS_TCP_POP_NAME,
		Object->STR_SS_TCP_POP_DOMAIN,
		Object->STR_SS_TCP_POP_SMTPHOST,
		Object->STR_SS_TCP_POP_POPHOST,
		Object->STR_SS_TCP_POP_USERNAME,
		Object->STR_SS_TCP_POP_PASSWORD,
		Object->CH_SS_TCP_POP_8BIT,
		Object->CH_SS_TCP_POP_APOP,
		Object->CH_SS_TCP_POP_DELMAILS,
		Object->CH_SS_TCP_POP_EXTSERVER,
		Object->STR_SS_TCP_POP_INVALID_SUBJECT,
		Object->PA_SS_TCP_POP_INVALID_TEXT,
		Object->STR_SS_TCP_POP_INVALID_NAME,
		Object->PA_SS_TCP_POP_INBOUND,
		Object->PA_SS_TCP_POP_OUTBOUND,
		Object->LV_SS_TCP_USERS,
		Object->BT_SS_TCP_USERS_NEW,
		Object->BT_SS_TCP_USERS_REMOVE,
		Object->LV_SS_TCP_USERS_IN_SERVER,
		Object->CY_SS_TCP_USERS_IN_SEARCH_NAMETYPE,
		Object->CY_SS_TCP_USERS_IN_SEARCH,
		Object->PO_SS_USERS_OUT_MOVETO,
		Object->STR_SS_USERS_OUT_FROM,
		Object->STR_SS_USERS_OUT_REPLYTO,
		Object->STR_SS_USERS_OUT_REPLYTO_DOMAIN,
		Object->BT_SS_TCP_USERS_IN_NEW,
		Object->BT_SS_TCP_USERS_IN_REMOVE,
		Object->LV_label_6,
		Object->BT_SS_FIDONEW,
		Object->BT_SS_FIDOADD,
		Object->BT_SS_FIDOREMOVE,
		Object->STR_SS_FIDOFIRSTZONEC,
		Object->STR_SS_FIDOLASTZONE,
		Object->STR_SS_FIDOFAKENET,
		Object->STR_SS_ADDRESS,
		Object->STR_SS_FIDOFIRSTAKA,
		Object->STR_SS_FIDOSECONDAKA,
		Object->STR_SS_FIDOECHOORIGIN,
		Object->STR_SS_FIDONETWORKORIGIN,
		Object->LV_SS_FIDOAREAS_AREASLIST,
		Object->STR_SS_FIDOAREAS_INPUT,
		Object->BT_SS_FIDOAREAS_NEW,
		Object->BT_SS_FIDOAREAS_ADD,
		Object->BT_SS_FIDOAREAS_REMOVE,
		Object->LV_FIDOAREAS_EXPORT,
		Object->STR_FIDOAREAS_ACCESSLEVEL,
		Object->STR_FIDOAREAS_ACCESSFLAGS,
		Object->STR_FIDOAREAS_DUPETABLE,
		Object->STR_FIDOAREAS_DESCRIPTION,
		Object->LV_SS_FIDOEXPORT_EXPORTTO,
		Object->STR_SS_FIDOEXPORT_INPUT,
		Object->BT_SS_FIDOEXPORT_NEW,
		Object->BT_SS_FIDOEXPORT_ADD,
		Object->BT_SS_FIDOEXPORT_REMOVE,
		Object->LV_FIDOEXPORT_AREAS,
		Object->CY_FIDOEXPORT_ARC,
		Object->RA_FIDOEXPORT_MAILTYPE,
		Object->RA_CC_STARTUPMODECC,
		Object->STR_FIDOEXPORT_ACCESSLEVEL,
		Object->STR_FIDOEXPORT_ACCESSFLAGS,
		Object->STR_FIDOEXPORT_AFPASSWORD,
		Object->LV_SS_FIDOROUTING_ROUTE,
		Object->STR_SS_FIDOROUTING_INPUT,
		Object->BT_SS_FIDOROUTING_INSERT,
		Object->BT_SS_FIDOROUTING_NEW,
		Object->BT_SS_FIDOROUTING_ADD,
		Object->BT_SS_FIDOROUTING_REMOVE,
		Object->STR_FIDOROUTING_ROUTETO,
		Object->RA_FIDOROUTING_MAILTYPEC,
		Object->CY_SS_Client_Sort,
		Object->STR_SS_Clients_Filter,
		Object->LV_SS_Clients,
		Object->BT_SS_Clients_New,
		Object->BT_SS_Clients_Edit,
		Object->BT_SS_Clients_Remove,
		Object->CY_label_3CC,
		Object->STR_SS_Clients_FilterCC,
		Object->LV_SS_Users,
		Object->BT_SS_Users_New,
		Object->BT_SS_Users_Edit,
		Object->BT_SS_Users_Remove,
		Object->BT_SS_Users_NewList,
		Object->LV_label_8,
		Object->STR_SS_ARC_INPUT,
		Object->BT_SS_ARC_NEW,
		Object->BT_SS_ARC_ADD,
		Object->BT_SS_ARC_REMOVE,
		Object->STR_SS_ARC_VIEWFORMAT,
		Object->STR_SS_ARC_TESTFORMAT,
		Object->STR_SS_ARC_TESTKEYWORD,
		Object->STR_SS_ARC_KEYWORDLINE,
		Object->STR_SS_ARC_KEYWORDCOLOMN,
		Object->STR_SS_ARC_PACKFORMAT,
		Object->STR_SS_ARC_EXTRACTFORMAT,
		Object->LV_SS_EDITORS,
		Object->STR_SS_EDITORS_INPUT,
		Object->BT_SS_EDITORS_NEW,
		Object->BT_SS_EDITORS_ADD,
		Object->BT_SS_EDITORS_REMOVE,
		Object->STR_SS_EDITORS_PATH,
		Object->CY_label_10,
		Object->LV_SS_LOGS,
		Object->STR_SS_LOGS_INPUT,
		Object->BT_SS_LOGS_NEW,
		Object->BT_SS_LOGS_ADD,
		Object->BT_SS_LOGS_REMOVE,
		Object->LV_SS_ACCESS,
		Object->STR_SS_ACCESS,
		Object->BT_SS_ACCESS_NEW,
		Object->BT_SS_ACCESS_REMOVE,
		Object->BT_SS_ACCEPT,
		Object->BT_SS_CANCEL,
		0
		);

	DoMethod(Object->WindowRegistration,
		MUIM_Window_SetCycleChain, Object->STR_RegNumber,
		Object->STR_Reg_Name,
		Object->STR_Reg_Location,
		Object->STR_Reg_Voicenumber,
		Object->STR_Reg_Modem,
		Object->STR_Reg_EMail,
		Object->BT_Reg_Save,
		0
		);

	DoMethod(Object->WindowLocked,
		MUIM_Window_SetCycleChain, Object->STR_CL_SystemPassword,
		Object->STR_CL_Encrypt,
		Object->BT_CL_OK,
		Object->BT_CL_CANCEL,
		0
		);

	DoMethod(Object->WindowNewClient,
		MUIM_Window_SetCycleChain, Object->GR_NC_DEFAILT_CONFIG,
		Object->STR_NC_PORTS,
		Object->STR_NC_FROMID,
		Object->STR_NC_TOID,
		Object->CY_NC_NETWORKTYPE,
		Object->RA_NC_STARTUPMODE,
		Object->CH_NC_HIDE,
		Object->CH_NC_INTERNALPORT,
		Object->CH_NC_ANSIONLY,
		Object->CH_NC_ADMINPORT,
		Object->CH_NC_USEPASSWORD,
		Object->STR_NC_PASSWORD,
		Object->STR_NC_ENCRYPT,
		Object->GR_NC_NETWORKCONFIG,
		Object->GR_NC_SERIAL,
		Object->PA_NC_SERIAL_DEVICE,
		Object->STR_NC_SERIAL_FROMUNIT,
		Object->STR_NC_SERIAL_TOUNIT,
		Object->CH_NC_SERIAL_SHARED,
		Object->STR_NC_SERIAL_IDLERATE,
		Object->STR_NC_SERIAL_ONLINERATE,
		Object->STR_NC_SERIAL_OFFLINERATE,
		Object->CY_NC_SERIAL_BITS,
		Object->CY_NC_SERIAL_PARITY,
		Object->CY_NC_SERIAL_PROTOCOLL,
		Object->CY_NC_SERIAL_STOPBITS,
		Object->CH_NC_SERIAL_8N1,
		Object->CH_NC_SERIAL_DSR,
		Object->CH_NC_SERIAL_DTR,
		Object->STR_NC_SERIAL_IOBUFFER,
		Object->STR_NC_SERIAL_CONNECT,
		Object->STR_NC_SERIAL_NOCARRIER,
		Object->STR_NC_SERIAL_NODIALTONE,
		Object->STR_NC_SERIAL_RING,
		Object->STR_NC_SERIAL_BUSY,
		Object->STR_NC_SERIAL_OK,
		Object->STR_NC_SERIAL_TIMEOUT,
		Object->STR_NC_SERIAL_ERROR,
		Object->STR_NC_SERIAL_INIT,
		Object->STR_NC_SERIAL_EXIT,
		Object->STR_NC_SERIAL_HANGUP,
		Object->STR_NC_SERIAL_HANGDOWN,
		Object->STR_NC_SERIAL_DIALPREFIX,
		Object->STR_NC_SERIAL_DIALSUFFIX,
		Object->CY_NC_SERIAL_DIALMODE,
		Object->STR_NC_SERIAL_ANSWER,
		Object->STR_NC_SERIAL_ANSWERTIMEOUT,
		Object->STR_NC_SERIAL_RINGS,
		Object->STR_NC_SERIAL_CONNECTTIMEOUT,
		Object->GR_NC_PAR,
		Object->PA_NC_PAR_DEVICE,
		Object->CY_NC_PAR_CABLETYPE,
		Object->STR_NC_PAR_FROMUNIT,
		Object->STR_NC_PAR_TOUNIT,
		Object->GR_NC_TCP,
		Object->PO_NC_TCPIP_HOST,
		Object->STR_NC_TCPIP_FROMPORT,
		Object->STR_NC_TCP_FROMIP,
		Object->STR_NC_TCP_TOIP,
		Object->STR_CC_TCPIP_TOPORT,
		Object->CY_NC_TCP_GETHOSTNAMEBY,
		Object->PA_NC_SCREENMODE,
		Object->CH_NC_USEWB,
		Object->CY_NC_COLORS,
		Object->PA_NC_SCREENFONT,
		Object->PA_NC_TERMFONT,
		Object->PA_NC_STARTUPSCRIPT,
		Object->CY_NC_STARTUPSCRIPT,
		Object->PA_NC_CONNECTSCRIPT,
		Object->CY_NC_CONNECTSCRIPT,
		Object->PA_NC_LOGOFFSCRIPT,
		Object->CY_NC_LOGOFFSCRIPT,
		Object->BT_NC_OTHERPORT,
		Object->BT_NC_ACCEPT,
		Object->BT_NC_CANCEL,
		0
		);

	DoMethod(Object->WindowUserEditor,
		MUIM_Window_SetCycleChain, Object->STR_USER_IDNUMBER,
		Object->STR_USER_USERNAME,
		Object->STR_USER_REALNAME,
		Object->STR_USER_STREET,
		Object->STR_USER_ZIPCODE,
		Object->STR_USER_CITY,
		Object->STR_USER_COUNTRY,
		Object->STR_USER_AREACODE,
		Object->STR_USER_PHONENUMBER,
		Object->STR_USER_MODEMNUMBER,
		Object->STR_USER_FAXNUMBER,
		Object->STR_USER_BIRTHDAY,
		Object->PO_USER_COMPUTER,
		Object->PO_USER_ACCESS,
		Object->PO_USER_ACCESSGROUP,
		Object->STR_USER_EXPIREDAY,
		Object->STR_USER_EXPIREDAYS,
		Object->CY_USER_CHARSET,
		Object->PO_USER_TEXTTYPE,
		Object->CY_USER_MOREMODE,
		Object->CY_USER_TIMEFORMAT,
		Object->CY_USER_DATEFORMAT,
		Object->CY_USER_LINEFEEDS,
		Object->CY_USER_TERMTABS,
		Object->CY_USER_TERMANSI,
		Object->CY_USER_TERMCOLORS,
		Object->STR_USER_TERMWIDTH,
		Object->STR_USER_TERMLENGTH,
		0
		);

	set(Object->WindowClientList,
		MUIA_Window_Open, TRUE
		);


	return(Object);
}

void DisposeApp(struct ObjApp * Object)
{
	MUI_DisposeObject(Object->App);
	FreeVec(Object);
}
