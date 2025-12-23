#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <stdio.h>
#include <novia/novia_portdata.h>

struct Library *NoviaSysBase;

void ioprintf(const char *string, ...)
{
	char *buffer=AllocVec(10000,MEMF_ANY|MEMF_CLEAR);
	if (buffer)
	{
		vsprintf(buffer, string, unsigned int(&string + 1));
		Writeio(buffer,-1);
		FreeVec(buffer);
	}
}

void main()
{
	if ((NoviaSysBase = OpenLibrary("noviasys.library", 0)))
	{
		struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
				struct SelectItem *select = (SelectItem *)cport->select_list.mlh_Head;
				struct Item item;
				while (select->ln_Succ)
				{
//					if (GetItem_byIP(select->dirIP,select->itemIP,&item,sizeof(ItemHeader)));
						ioprintf("selected\n");
//						ioprintf("%-40d\n",item.itemheader.Title);
					select = select->ln_Succ;
				}
		CloseLibrary(NoviaSysBase);
	}
}