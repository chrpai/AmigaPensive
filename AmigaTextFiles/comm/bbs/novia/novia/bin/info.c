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
		ULONG ulong;
		struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
		if ((ulong = (ULONG) ndos_info(cport->argstring[1])))
		{
			ioprintf("can't rename %s.\n",cport->argptr);
			PrintDosError((LONG)ulong);
		}
		else
			ioprintf("%s renamed.\n",cport->argptr);
		CloseLibrary(NoviaSysBase);
	}
}