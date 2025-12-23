#include <libraries/mui.h>
#include <pragma/muimaster_lib.h>
#include <clib/exec_protos.h>
#include <exec/memory.h>
#include <clib/alib_protos.h>

struct ObjApp
{
	APTR	App;
	APTR	Window1;
	APTR	BT_System_Service;
	APTR	BT_Base;
	APTR	BT_Filebase;
	APTR	BT_Chat_Channels;
	APTR	BT_News;
	APTR	BT_PFiles;
	APTR	BT_GFiles;
	APTR	LV_Client_List;
	APTR	BT_Userinfo;
	APTR	BT_Portinfo;
	APTR	BT_Mailbox;
	APTR	BT_Select_All;
	APTR	BT_Select_Nothing;
	APTR	BT_Config;
	APTR	BT_Online;
	APTR	BT_Offline;
	APTR	CY_Port_Window;
	APTR	WindowClientConfig;
	APTR	LV_ClientList2;
	APTR	BT_Remove;
	APTR	BT_New;
	APTR	BT_Edit;
	APTR	WindowRequest;
	APTR	TX_label_1;
	APTR	BT_Yes;
	APTR	BT_No;
	char *	STR_TX_label_1;
	char *	CY_Port_WindowContent[3];
};


extern struct ObjApp * CreateApp(void);
extern void DisposeApp(struct ObjApp *);
