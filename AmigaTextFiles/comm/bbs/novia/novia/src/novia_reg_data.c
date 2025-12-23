#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifdef _DCC
#define __inline
#endif

#include "novia_reg_data.h"

struct RegApp * CreateRegApp(void)
{
	struct RegApp * Object;

	APTR	GROUP_ROOT_0, GR_grp_28, LA_Reg_IDNumber, LA_Reg_Name, LA_Reg_Location;
	APTR	LA_Reg_VoiceNo, LA_Reg_Modemno, LA_Reg_EMail, GR_grp_29, LA_label_25;

	if (!(Object = AllocVec(sizeof(struct RegApp), MEMF_PUBLIC|MEMF_CLEAR)))
		return(NULL);

	Object->STR_TX_label_7 = "Welcome to\nNOViA V0.00.05, "__DATE__" "__TIME__"\n Please enter your Registration Number";

	Object->TX_label_7 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, Object->STR_TX_label_7,
	End;

	LA_Reg_IDNumber = Label("Registration Number");

	Object->STR_RegNumber = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_RegNumber",
	End;

	LA_Reg_Name = Label("Your Name");

	Object->STR_Reg_Name = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Name",
	End;

	LA_Reg_Location = Label("Location");

	Object->STR_Reg_Location = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Location",
	End;

	LA_Reg_VoiceNo = Label("Your Voicenumber");

	Object->STR_Reg_Voicenumber = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Voicenumber",
		MUIA_String_Accept, "0123456789.",
	End;

	LA_Reg_Modemno = Label("1. Modemline or Telnet-Address");

	Object->STR_Reg_Modem = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_Modem",
		MUIA_String_Accept, "0123456789.+-",
	End;

	LA_Reg_EMail = Label("Your EMail Address");

	Object->STR_Reg_EMail = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Reg_EMail",
	End;

	GR_grp_28 = GroupObject,
		MUIA_HelpNode, "GR_grp_28",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Your Registrarion Details",
		MUIA_Group_Columns, 2,
		Child, LA_Reg_IDNumber,
		Child, Object->STR_RegNumber,
		Child, LA_Reg_Name,
		Child, Object->STR_Reg_Name,
		Child, LA_Reg_Location,
		Child, Object->STR_Reg_Location,
		Child, LA_Reg_VoiceNo,
		Child, Object->STR_Reg_Voicenumber,
		Child, LA_Reg_Modemno,
		Child, Object->STR_Reg_Modem,
		Child, LA_Reg_EMail,
		Child, Object->STR_Reg_EMail,
	End;

	LA_label_25 = Label("Press Save, if you are finish");

	Object->BT_Reg_Save = SimpleButton("Save");

	GR_grp_29 = GroupObject,
		MUIA_HelpNode, "GR_grp_29",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Horiz, TRUE,
		Child, LA_label_25,
		Child, Object->BT_Reg_Save,
	End;

	GROUP_ROOT_0 = GroupObject,
		Child, Object->TX_label_7,
		Child, GR_grp_28,
		Child, GR_grp_29,
	End;

	Object->WI_label_0 = WindowObject,
		MUIA_Window_Title, "NOViA REGiSTRATiON",
		MUIA_Window_ID, MAKE_ID('0', 'W', 'I', 'N'),
		WindowContents, GROUP_ROOT_0,
	End;

	Object->App = ApplicationObject,
		MUIA_Application_Author, "NONE",
		MUIA_Application_Base, "NONE",
		MUIA_Application_Title, "NONE",
		MUIA_Application_Version, "$VER: NONE XX.XX (XX.XX.XX)",
		MUIA_Application_Copyright, "NOBODY",
		MUIA_Application_Description, "NONE",
//		MUIA_HelpFile, "novia_client.guide",
		SubWindow, Object->WI_label_0,
	End;


	if (!Object->App)
	{
		FreeVec(Object);
		return(NULL);
	}

	DoMethod(Object->WI_label_0,
		MUIM_Window_SetCycleChain, Object->STR_RegNumber,
		Object->STR_Reg_Name,
		Object->STR_Reg_Location,
		Object->STR_Reg_Voicenumber,
		Object->STR_Reg_Modem,
		Object->STR_Reg_EMail,
		Object->BT_Reg_Save,
		0
		);

	set(Object->WI_label_0,
		MUIA_Window_Open, TRUE
		);

	DoMethod(Object->WI_label_0,	MUIM_Notify,MUIA_Window_CloseRequest, TRUE,Object->App,2,MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
	DoMethod(Object->BT_Reg_Save,	MUIM_Notify,MUIA_Pressed, FALSE,Object->App,2,MUIM_Application_ReturnID, REGID_SAVE);

	return(Object);
}

void DisposeRegApp(struct RegApp * Object)
{
	MUI_DisposeObject(Object->App);
	FreeVec(Object);
}

