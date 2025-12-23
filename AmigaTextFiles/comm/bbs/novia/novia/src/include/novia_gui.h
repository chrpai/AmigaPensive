#ifndef NOVIA_GUI_H
#define NOVIA_GUI_H
#define NOVIA_GUI_H_VERSION "$VER: 0.14 (01.06.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

#ifndef NOVIA_PORTDATA_H
#include <novia/novia_portdata.h>
#endif

#ifndef NOVIA_USERDATA_H
#include <novia/novia_userdata.h>
#endif

#ifndef NOVIA_USERLIST_H
#include <novia/novia_userlist.h>
#endif

#define GUIID_MINMIN			101
#define GUIID_SYSTEM_SERVICE	101
#define GUIID_BASE				102
#define GUIID_FILEBASE			103
#define GUIID_CHAT_CHANNELS		104
#define GUIID_NEWS				105
#define GUIID_PFILES			106
#define GUIID_GFILES			107
#define GUIID_VOTES				108
#define GUIID_AUSLASTUNG		109
#define GUIID_CLIENT_LIST		110
#define GUIID_USERINFO			111
#define GUIID_PORTINFO			112
#define GUIID_MAILBOX			113
#define GUIID_SELECT_ALL		114
#define GUIID_SELECT_NOTHING	115
#define GUIID_CONFIG			116
#define GUIID_ONLINE			117
#define GUIID_OFFLINE			118
#define GUIID_REMOVE			119
#define GUIID_LOAD				120
#define GUIID_ABOUT				121
#define GUIID_ABOUTMUI			122
#define GUIID_LOCKCONSULE		123
#define GUIID_RESTARTSERVER		124
#define GUIID_RESETSERVER		125
#define GUIID_QUIT				126
#define GUIID_LOCALLOGIN		127
#define GUIID_NONEWUSERS		128
#define GUIID_BUSYALLPORTS		129
#define GUIID_DISABLEBASE		130
#define GUIID_CONFIGBASE		131
#define GUIID_DISABLEUDBASE		132
#define GUIID_CONFIGUDBASE		133
#define GUIID_CHATCHANNELS		134
#define GUIID_DISABLECC			135
#define GUIID_DISABLENEWS		136
#define GUIID_CONFIGNEWS		137
#define GUIID_DISABLEPFILES		138
#define GUIID_CONFIGPFILES		139
#define GUIID_DISABLEGFILES		140
#define GUIID_CONFIGGFILES		141
#define GUIID_DISABLEVOTES		142
#define GUIID_CONFIGCONFIG		143
#define GUIID_SS_CLIENTS_NEW	144
#define GUIID_SS_CLIENTS_EDIT	145
#define GUIID_SS_CLIENTS_REMOVE	146
#define GUIID_SS_USERS_NEW		147
#define GUIID_SS_USERS_EDIT		148
#define GUIID_SS_USERS_REMOVE	149
#define GUIID_SS_ACCEPT			150
#define GUIID_SS_CANCEL			151

#define GUIID_CY_NETWORKCON		153
#define GUIID_SCREENMODE		154
#define GUIID_NEWREGISTER		155
#define GUIID_SS_OPTIONS_		156

/****************** STRING RETURN ID's **********************/

#define GUIID_STR_Address		157
#define GUIID_STR_PA_Screenmode	158
#define GUIID_STR_PA_ScreenFont	159
#define GUIID_STR_PA_TerminalFont	160
#define GUIID_STR_PA_DeviceName		161
#define GUIID_STR_SerConfig_UnitnoC				162
#define GUIID_STR_ClientConfig_Baudrateidle		163
#define GUIID_STR_Baudrateonline				164
#define GUIID_STR_ClientConfig_Baudrateonline	165
#define GUIID_STR_SerConfig_IOBuffer			166
#define GUIID_STR_Modem_CONNECT					167
#define GUIID_STR_Modem_NOCARRIER				168
#define GUIID_STR_Modem_NODIALTONE				169
#define GUIID_STR_Modem_RING					170
#define GUIID_STR_Modem_BUSY					171
#define GUIID_STR_Modem_OK						172
#define GUIID_STR_Modem_TIMEOUT					173
#define GUIID_STR_Modem_ERROR					174
#define GUIID_STR_Commands_init					175
#define GUIID_STR_Commands_exit					176
#define GUIID_STR_Commands_hangup				177
#define GUIID_STR_Dial_prefix					178
#define GUIID_STR_Dial_suffix					179
#define GUIID_STR_Answer_anscmd					180
#define GUIID_STR_PA_DeviceParallel				181
#define GUIID_STR_SystemName					182
#define GUIID_STR_RegLocation					183
#define GUIID_STR_MainPortNumber				184
#define GUIID_STR_UUCPDomain					185
#define GUIID_STR_SysopAccountNo				186
#define GUIID_STR_DefaultAreaCode				187
#define GUIID_STR_DefaultLocationNo				188
#define GUIID_STR_DefaultCountryNo				189
#define GUIID_STR_PA_OptionsFont				190
#define GUIID_STR_SS_Clients_Filter				191
#define GUIID_STR_SS_Clients_FilterCC			192
#define GUIID_STR_RegNumber						193
#define GUIID_STR_Reg_Name						194
#define GUIID_STR_Reg_Location					195
#define GUIID_STR_Reg_Voicenumber				196
#define GUIID_STR_Reg_Modem						197
#define GUIID_STR_Reg_EMail						198
#define GUIID_CH_CC_loadatstart 				199
#define GUIID_BT_CC_Save						200
#define GUIID_BT_CC_Use							201
#define GUIID_BT_CC_Cancel						202
#define GUIID_DC_CLIENT_LIST					203
#define GUIID_DC_SS_CLIENT_LIST					204
#define GUIID_DC_SS_USERS						205
#define GUIID_CL_OK								206
#define GUIID_CL_CANCEL							207
#define GUIID_SS_CLOSEWIN						208
#define GUIID_USER_CLOSEWIN						209
#define GUIID_SS_TCP_MAILBOX_NEW				210
#define GUIID_SS_TCP_MAILBOX_REMOVE				212
#define GUIID_SS_TCP_MAILBOX_USER_NEW			215
#define GUIID_SS_TCP_MAILBOX_USER_CHANGENAME	216
#define GUIID_SS_TCP_MAILBOX_USER_REMOVE		217
#define GUIID_SS_TCP_POP3_NEW					220
#define GUIID_SS_TCP_POP3_REMOVE				221
#define GUIID_SS_TCP_MAILBOX_NAME				222
#define GUIID_CC_CLOSEWIN						225

#define GUIID_REMINDER_NEW						400
#define GUIID_REMINDER_ADD						401
#define GUIID_REMINDER_REMOVE					402
#define GUIID_REMINDER_TIME						403
#define GUIID_REMINDER_INTERNAL					404
#define GUIID_REMINDER_INTERVAL					405
#define GUIID_REMINDER_DAILY					406
#define GUIID_REMINDER_MON						407
#define GUIID_REMINDER_THU						408
#define GUIID_REMINDER_WEN						409
#define GUIID_REMINDER_TUR						410
#define GUIID_REMINDER_FRI						411
#define GUIID_REMINDER_SAT						412
#define GUIID_REMINDER_SUN						413
#define GUIID_REMINDER_WEEKLY					414
#define GUIID_REMINDER_1WEEK					415
#define GUIID_REMINDER_2WEEK					416
#define GUIID_REMINDER_3WEEK					417
#define GUIID_REMINDER_4WEEK					418
#define GUIID_REMINDER_5WEEK					419
#define GUIID_REMINDER_MONTHLY					420
#define GUIID_REMINDER_1MONTH					421
#define GUIID_REMINDER_2MONTH					422
#define GUIID_REMINDER_3MONTH					423
#define GUIID_REMINDER_4MONTH					424
#define GUIID_REMINDER_5MONTH					425
#define GUIID_REMINDER_6MONTH					426
#define GUIID_REMINDER_7MONTH					427
#define GUIID_REMINDER_8MONTH					428
#define GUIID_REMINDER_9MONTH					429
#define GUIID_REMINDER_10MONTH					430
#define GUIID_REMINDER_11MONTH					431
#define GUIID_REMINDER_12MONTH					432
#define GUIID_REMINDER_USEDATE					433
#define GUIID_REMINDER_DATE						434
#define GUIID_REMINDER_TYPE						435
#define GUIID_REMINDER_CLOSEPORT				436
#define GUIID_REMINDER_NAME						437
#define GUIID_REMINDER_INTERVALTYPE				480
#define GUIID_REMINDER_HOLIDAYS					481
#define GUIID_REMINDER_DATETYPE					482
#define GUIID_REMINDER_COPY						483
#define GUIID_REMINDER_UPDATE					484
#define GUIID_REMINDER_TEST						485

#define GUIMSG_CLOSE_FRONT_END					1000
#define GUIMSG_CLOSE_MAINPORT					1001
#define GUIMSG_CLOSE_FAIL20						1002
#define GUIMSG_CLOSE_OK							1003
#define GUIMSG_CLOSE_WAIT_FOR_CLOSE_CMD	0x104

#define GUIMSG_CLIENT_ADD						2000
#define GUIMSG_CLIENT_REMOVE					2001
#define GUIMSG_CLIENT_CHANGE					2002	// change status

#define GUIMSG_CLIENT_NEW						2010
#define GUIMSG_CLIENT_EDIT						2011

#define GUIMSG_CLIENT_CANCEL					2020
#define GUIMSG_CLIENT_SAVE						2021
#define GUIMSG_CLIENT_USE						2022

#define GUIMSG_CLIENT_ONLINE					2030
#define GUIMSG_CLIENT_OFFLINE					2031
#define GUIMSG_CLIENT_UNLOAD					2032

#define GUIMSG_CLIENTLIST_REFRESH				2100

#define GUIMSG_CLIENT_TOFRONT					2200

#define GUIMSG_REPLY_ERROR						10000
#define GUIMSG_REPLY_TIMEOUT					10001

#define GUIMSG_REPLY							10005

#define GUIMSG_FRONTEND_INIT					20000
#define GUIMSG_REPLY_FE_INIT					20001
#define GUIMSG_FE_STRUCT_INIT					20005
#define GUIMSG_FE_STRUCT_REFRESH				20006
#define GUIMSG_INIT_FAIL						20010

#define GUIMSG_ABOUT							25000
#define GUIMSG_QUIT								25001
#define GUIMSG_LOCKCONSULE						25002

#define GUIMSG_MPCONFIG_LOAD					30005
#define GUIMSG_MPCONFIG_SAVE					30006
#define GUIMSG_MPCONFIG_USE						30007

#define GUIMSG_REDRAW_REMINDERLIST				30050

#define GUICMD_LIST_CLEAR						30200
#define GUICMD_LIST_REPLACE						30201
#define GUICMD_LIST_CHANGE_ENTRYS				30202


/************************** PRIVATE NOVIA_GUI ****************/


#define FD_MUI_SCONFIG_ADDRESS_LOCAL		0
#define FD_MUI_SCONFIG_ADDRESS_SERIAL		1
#define FD_MUI_SCONFIG_ADDRESS_PARALLEL		2
#define FD_MUI_SCONFIG_ADDRESS_TCPIP		3
#define FD_MUI_SCONFIG_ADDRESS_ENVOY		4
#define FD_MUI_SCONFIG_ADDRESS_NOVELL		5
#define FD_MUI_SCONFIG_ADDRESS_MSN			6


/************************** PUPLIC STRUCTURES ****************/

struct GUIMsg
{
	struct 	Message msg;			// Std Exec-Message Structure
	struct 	GUIMsg *ln_Succ;
	struct	GUIMsg *ln_Pred;		/* Node fot internal use.
										Falls irgendein Programm vergessen hat, eine Message zurück
										zu senden, kann der Speicher, falls dieser knapp ist
										mit hilfe der gui_msg_list wieder frei gegeben werden.*/
	ULONG			cmdid;			// Type of Message
	ULONG			cmdid2;
	APTR			data;			/* pointer to data. Da das GUIFrontend und der Mainport in
										normalerweise auf einem Rechner laufen, brauchen die Daten
										nicht an die Message angehängt werden. */
									
};

struct GUIMsgList
{
	struct GUIMsg *first;
	struct GUIMsg *last;
	ULONG  entrys;
};

struct List_Entry
{
	struct Node *	ln_Succ;	// Nodestruct
	struct Node *	ln_Pred;
	ULONG			entryno;	// number of entry in List
	APTR			data;		// pointer to PortData or UserData... etc
	char			text[80];	// String in List
};

struct GUIFrontendOld {
	struct 	MainPortConfig	*MainPortConfig;	// MainPortConfiguration
	struct	List			*clist;				// ClientList
	struct	PortData		*cport;				// Pointer of cport-structure
	struct	List			*lockStructures;	// Pointer of lock-structure
	ULONG					*numberofclients;	// number of clients
	};

struct GUIFrontend
{
	struct	PortData *mport;
	char	ProgramClose;
	char	GUIOpen;
};

#endif



