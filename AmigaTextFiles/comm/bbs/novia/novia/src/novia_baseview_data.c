#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifdef _DCC
#define __inline
#endif

#include "novia_baseview_data.h"
#include "novia_baseview_dataExtern.h"

struct BaseViewApp * CreateBaseViewApp(char *text)
{
	struct BaseViewApp * Object;

	APTR	GROUP_ROOT_0, GR_grp_0, GR_grp_1, Space_2;

	static const struct Hook DisplayItemHeaderEntryHook = { { NULL,NULL },(VOID *)DisplayItemHeaderEntry,NULL,NULL };
	static const struct Hook RemoveItemHeaderEntryHook = { { NULL,NULL },(VOID *)RemoveItemHeaderEntry,NULL,NULL };

	if (!(Object = AllocVec(sizeof(struct BaseViewApp), MEMF_PUBLIC|MEMF_CLEAR)))
		return(NULL);

	Object->LV_BASEVIEW_HEADER = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_Format, "P=\33r\338,",
		MUIA_List_DisplayHook, &DisplayItemHeaderEntryHook,
		MUIA_List_DestructHook, &RemoveItemHeaderEntryHook,
	End;

	Object->LV_BASEVIEW_HEADER = ListviewObject,
		MUIA_HelpNode, "LV_BASEVIEW_HEADER",
		MUIA_Weight, 20,
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, Object->LV_BASEVIEW_HEADER,
	End;

	Object->BASEVIEW_TEXT =	NListviewObject,
		MUIA_NListview_NList,
			Object->MAILTEXT = MailtextObject,
				MUIA_Mailtext_ForbidContextMenu, FALSE,
				MUIA_Font,                       MUIV_Font_Fixed,
				MUIA_Frame,                      MUIV_Frame_InputList,
				MUIA_NList_Input,                FALSE,
				MUIA_NList_MultiSelect,          FALSE,
			End,
		MUIA_CycleChain, TRUE,
	End;

	Object->REC_label_0 = RectangleObject,
		MUIA_Rectangle_HBar, TRUE,
		MUIA_FixHeight, 8,
		MUIA_Scale_Horiz, TRUE,
	End;

	Object->BT_PREV_REPLY = SimpleButton("P_rev Reply");

	Object->BT_NEXT_REPLY = SimpleButton("_Next Reply");

	Object->BT_DEL_REPLY = SimpleButton("Delete Reply");

	Object->BT_EDIT = SimpleButton("Edit");

	Object->BT_NEW = SimpleButton("New");

	Space_2 = HVSpace;

	Object->BT_PREV_POST = SimpleButton("Pr_ev Post");

	Object->BT_NEXT_POST = SimpleButton("Next P_ost");

	Object->BT_DEL_POST = SimpleButton("Delete Post");

	GR_grp_1 = GroupObject,
		MUIA_HelpNode, "GR_grp_1",
		MUIA_Group_Horiz, TRUE,
		Child, Object->BT_PREV_REPLY,
		Child, Object->BT_NEXT_REPLY,
		Child, Object->BT_DEL_REPLY,
		Child, Object->BT_EDIT,
		Child, Object->BT_NEW,
		Child, Space_2,
		Child, Object->BT_PREV_POST,
		Child, Object->BT_NEXT_POST,
		Child, Object->BT_DEL_POST,
	End;

	GR_grp_0 = GroupObject,
		MUIA_HelpNode, "GR_grp_0",
		Child, Object->LV_BASEVIEW_HEADER,
		Child, Object->REC_label_0,
		Child, Object->BASEVIEW_TEXT,
		Child, GR_grp_1,
	End;

	GROUP_ROOT_0 = GroupObject,
		Child, GR_grp_0,
	End;

	Object->WI_label_0 = WindowObject,
		MUIA_Window_Title, "Message Base Viewer",
		MUIA_Window_ID, MAKE_ID('0', 'W', 'I', 'N'),
		WindowContents, GROUP_ROOT_0,
	End;

	Object->App = ApplicationObject,
		MUIA_Application_Author, "Thorten Gehler",
		MUIA_Application_Base, "Novia Message Viewer",
		MUIA_Application_Title, "Novia Message Viewer",
		MUIA_Application_Version, "$VER: Novia Message Viewer 01.00 (16.01.1999)",
		MUIA_Application_Copyright, "(c) 1999 by Thorsten Gehler",
		MUIA_Application_Description, "NONE",
		MUIA_HelpFile, "novia_client.guide",
		SubWindow, Object->WI_label_0,
	End;


	if (!Object->App)
	{
		FreeVec(Object);
		return(NULL);
	}

	DoMethod(Object->BT_PREV_REPLY,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_REPLY_REPLY
		);

	DoMethod(Object->BT_NEXT_REPLY,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_REPLY_NEXT
		);

	DoMethod(Object->BT_DEL_REPLY,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_REPLY_REMOVE
		);

	DoMethod(Object->BT_EDIT,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_EDIT
		);

	DoMethod(Object->BT_NEW,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_NEW
		);

	DoMethod(Object->BT_PREV_POST,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_POST_PREV
		);

	DoMethod(Object->BT_NEXT_POST,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_POST_NEXT
		);

	DoMethod(Object->BT_DEL_POST,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, GUIID_BASEVIEWER_POST_REMOVE
		);

	DoMethod(Object->REC_label_0,
		MUIM_Notify, MUIA_Pressed, FALSE,
		Object->App,
		2,
		MUIM_Application_ReturnID, 10
		);

	DoMethod(Object->WI_label_0,
		MUIM_Window_SetCycleChain, Object->LV_BASEVIEW_HEADER,
		Object->BASEVIEW_TEXT,
		Object->BT_PREV_REPLY,
		Object->BT_NEXT_REPLY,
		Object->BT_DEL_REPLY,
		Object->BT_EDIT,
		Object->BT_NEW,
		Object->REC_label_0,	
		Object->BT_PREV_POST,
		Object->BT_NEXT_POST,
		Object->BT_DEL_POST,
		0
		);

	DoMethod(Object->WI_label_0,	MUIM_Notify,MUIA_Window_CloseRequest, TRUE,Object->App,2,MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);

	set(Object->WI_label_0,
		MUIA_Window_Open, TRUE
		);


	return(Object);
}

void DisposeBaseViewApp(struct BaseViewApp * Object)
{
	MUI_DisposeObject(Object->App);
	FreeVec(Object);
}
