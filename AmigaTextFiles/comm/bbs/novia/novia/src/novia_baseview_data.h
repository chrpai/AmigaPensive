#include <libraries/mui.h>
#include <pragma/muimaster_lib.h>
#include <clib/exec_protos.h>
#include <exec/memory.h>
#include <clib/alib_protos.h>
#include <mui/Mailtext_mcc.h>
#include <mui/NListview_mcc.h>

struct MessageHeaderEntry
{
	char *header;		// Subject:, To: ....
	char *text;			// <Subject>,<Name of Author> ...
};

struct BaseViewApp
{
	APTR	App;
	APTR	WI_label_0;
	APTR	LV_BASEVIEW_HEADER;
	APTR	BASEVIEW_TEXT;
	APTR	REC_label_0;
	APTR	MAILTEXT;
	APTR	BT_PREV_REPLY;
	APTR	BT_NEXT_REPLY;
	APTR	BT_DEL_REPLY;
	APTR	BT_EDIT;
	APTR	BT_NEW;
	APTR	BT_PREV_POST;
	APTR	BT_NEXT_POST;
	APTR	BT_DEL_POST;
};

#define GUIID_BASEVIEWER_EDIT 1
#define GUIID_BASEVIEWER_NEW 2
#define GUIID_BASEVIEWER_POST_NEXT 3
#define GUIID_BASEVIEWER_POST_PREV 4
#define GUIID_BASEVIEWER_REPLY_REPLY 5
#define GUIID_BASEVIEWER_REPLY_NEXT 6
#define GUIID_BASEVIEWER_REPLY_REMOVE 7
#define GUIID_BASEVIEWER_POST_REMOVE 8
#define GUIID_BASEVIEWER_RECTANGLE 9

extern struct BaseViewApp * CreateBaseViewApp(char *text);
extern void DisposeBaseViewApp(struct BaseViewApp *);
