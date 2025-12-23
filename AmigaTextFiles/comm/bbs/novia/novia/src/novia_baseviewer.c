#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifdef _DCC
#define __inline
#endif

#include <exec/types.h>
#include <stdlib.h>
#include <intuition/screens.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <novia/novia_UserList.h>
#include <novia/novia_PortData.h>
#include <novia/novia_config.h>
#include <novia/novia_mail.h>
#include <novia/novia_subboard.h>
#include <novia/novia_types.h>
#include <novia/novia_misc.h>
#include <novia/novia_message.h>
#include <exec/memory.h>
#include <dos/stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <novia/novia_funcs.h>
#include "novia_baseview_data.h"

struct MyNode {
	struct MyNode 	*ln_Succ;
	struct MyNode	*ln_Pred;
	UBYTE			ln_Type;
	BYTE			ln_Pri;
	BYTE 			*ln_Name;
	ULONG			IPNumber;
};

LONG DisplayListListBase(REG(a2) char **array,REG(a1) struct ViewItem *item);
LONG DisplayHook_MsgBase(REG(a2) char **array,REG(a1) struct ViewItem *item);
LONG DisplayItemHeaderEntry(REG(a2) char **array,REG(a1) MessageHeaderEntry *msgheader);
LONG RemoveItemHeaderEntry(REG(a2) char **array,REG(a1) MessageHeaderEntry *msgheader);
void InsertMessageHeader(APTR List,char *header, char *text);


LONG DisplayListBase(REG(a2) char **array,REG(a1) struct ItemHeader *item)
{
	*array++ = item->Title;
	return(0);
}

LONG DisplayListBaseRemove(REG(a2) char **array,REG(a1) struct ItemHeader *item)
{
	if (item)FreeVec(item);
	return(0);
}

LONG DisplayHook_MsgBase(REG(a2) char **array,REG(a1) struct ViewItem *item) 
{
	*array++ = item->Title;
	return(0);
}

LONG DisplayItemHeaderEntry(REG(a2) char **array,REG(a1) MessageHeaderEntry *msgheader)
{
	*array++ = msgheader->header;
	*array = msgheader->text;
//	printf("header: %s\n",msgheader->header);
//	printf("text  : %s\n",msgheader->text);
	return(0);
}

LONG RemoveItemHeaderEntry(REG(a2) char **array,REG(a1) MessageHeaderEntry *msgheader)
{
	if (msgheader)
	{
		if (msgheader->header)FreeVec(msgheader->header);
		if (msgheader->text)FreeVec(msgheader->text);
		FreeVec(msgheader);
	}
	return(0);
}

void InsertMessageHeader(APTR List,char *header, char *text)
{
	MessageHeaderEntry *msgheader;
	if ((msgheader=AllocVec(sizeof(MessageHeaderEntry),MEMF_ANY|MEMF_CLEAR)))
	{
		if ((msgheader->header=AllocVec(strlen(header)+1,MEMF_ANY|MEMF_CLEAR)))
		{
			if ((msgheader->text=AllocVec(strlen(text)+1,MEMF_ANY|MEMF_CLEAR)))
			{
				strcpy(msgheader->header,header);
				strcpy(msgheader->text,text);
				DoMethod(List, MUIM_List_Insert,&msgheader, 1, MUIV_List_Insert_Bottom);
			}
			else
			{
				FreeVec(msgheader->header);
				FreeVec(msgheader);
			}
		}
		else
			FreeVec(msgheader);
	}

}




