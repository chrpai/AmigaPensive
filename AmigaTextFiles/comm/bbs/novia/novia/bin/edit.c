#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <stdio.h>
#include <novia/novia_portdata.h>

struct Library *NoviaSysBase;

void ioprintf(const char *string, ...)
{
	char *buffer = AllocVec(10000,MEMF_ANY|MEMF_CLEAR);
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
		ItemEntryBlock *ieb;
		if ((ieb = ndos_get_ieb(cport->argstring[1])))
		{
			ioprintf("edit: %d\n",ndos_editflags(cport->argstring[1]));
		}
		CloseLibrary(NoviaSysBase);
	}
}