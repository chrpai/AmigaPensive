#include <libraries/mui.h>
#include <pragma/muimaster_lib.h>
#include <clib/exec_protos.h>
#include <exec/memory.h>
#include <clib/alib_protos.h>

#define REGID_SAVE 1111

struct RegApp
{
	APTR	App;
	APTR	WI_label_0;
	APTR	TX_label_7;
	APTR	STR_RegNumber;
	APTR	STR_Reg_Name;
	APTR	STR_Reg_Location;
	APTR	STR_Reg_Voicenumber;
	APTR	STR_Reg_Modem;
	APTR	STR_Reg_EMail;
	APTR	BT_Reg_Save;
	char *	STR_TX_label_7;
};


extern struct RegApp * CreateRegApp(void);
extern void DisposeRegApp(struct RegApp *);

